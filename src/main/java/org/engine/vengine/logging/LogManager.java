
/*
 * V-Engine
 * Copyright (C) 2025
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package org.engine.vengine.logging;

import java.util.Map;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicLong;

/**
 * Global manager holding loggers and background writer thread.
 */
public final class LogManager {
    private static final LogManager INSTANCE = new LogManager();

    public static LogManager get() { return INSTANCE; }

    private final ConcurrentMap<String, Logger> loggers = new ConcurrentHashMap<>();
    private volatile LoggerConfig config = new LoggerConfig();
    private volatile BlockingDeque<LogRecord> queue;
    private volatile ExecutorService writer;
    private final AtomicLong dropped = new AtomicLong(0);

    private LogManager() { applyConfig(config); }

    public synchronized void applyConfig(LoggerConfig cfg) {
        // close previous appenders if any
        if (writer != null) {
            writer.shutdownNow();
            writer = null;
        }
        if (queue != null) {
            queue.clear();
            queue = null;
        }

        this.config = cfg;

        if (cfg.isAsync()) {
            this.queue = new LinkedBlockingDeque<>(cfg.getQueueCapacity());
            this.writer = Executors.newSingleThreadExecutor(r -> {
                Thread t = new Thread(r, "Engine-Logger-Writer");
                t.setDaemon(true);
                return t;
            });
            ((ExecutorService) writer).submit(this::writerLoop);
        } else {
            this.queue = null;
            this.writer = null;
        }

        // update existing loggers' threshold
        for (Logger l : loggers.values()) l.updateThreshold(cfg.getLevel());
    }

    public Logger getLogger(Class<?> c) { return getLogger(c.getName()); }
    public Logger getLogger(String category) {
        return loggers.computeIfAbsent(category, name -> new Logger(name, config.getLevel(), this));
    }

    void enqueue(LogRecord record) {
        if (!config.isAsync()) {
            // synchronous: deliver directly
            for (Appender a : config.getAppenders()) {
                try { a.append(record); } catch (Throwable ignored) {}
            }
            return;
        }
        // async: try to add; if full drop oldest then add
        BlockingDeque<LogRecord> q = queue;
        if (q == null) return;
        boolean offered = q.offerLast(record);
        if (!offered) {
            // drop oldest to make room
            q.pollFirst();
            boolean offered2 = q.offerLast(record);
            if (!offered2) {
                dropped.incrementAndGet();
            }
        }
    }

    private void writerLoop() {
        BlockingDeque<LogRecord> q = queue;
        while (!Thread.currentThread().isInterrupted()) {
            try {
                LogRecord r = q.pollFirst(500, TimeUnit.MILLISECONDS);
                if (r != null) {
                    for (Appender a : config.getAppenders()) {
                        try { a.append(r); } catch (Throwable ignored) {}
                    }
                } else {
                    // flush periodically
                    for (Appender a : config.getAppenders()) {
                        try { a.flush(); } catch (Throwable ignored) {}
                    }
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            } catch (Throwable t) {
                System.err.println("Logger writer loop failed: " + t.getMessage());
            }
        }
        // draining remaining
        LogRecord r;
        while ((r = q.pollFirst()) != null) {
            for (Appender a : config.getAppenders()) {
                try { a.append(r); } catch (Throwable ignored) {}
            }
        }
        for (Appender a : config.getAppenders()) {
            try { a.flush(); a.close(); } catch (Throwable ignored) {}
        }
    }

    public long getDroppedCount() { return dropped.get(); }

    public LoggerConfig getConfig() { return config; }

    public void shutdown() {
        if (writer != null) {
            writer.shutdownNow();
            writer = null;
        }
        for (Appender a : config.getAppenders()) {
            try { a.flush(); a.close(); } catch (Throwable ignored) {}
        }
    }
}

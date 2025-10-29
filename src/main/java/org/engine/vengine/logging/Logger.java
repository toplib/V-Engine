
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

import java.util.Objects;
import java.util.function.Supplier;

/**
 * Main logger class.
 *
 * Usage:
 *   private static final Logger LOG = LogManager.get().getLogger(MyClass.class);
 *   LOG.info("Loaded mesh {}", () -> mesh.getName()); // use supplier for expensive-to-build messages
 */
public final class Logger {
    private final String category;
    private volatile LogLevel threshold;
    private final LogManager manager;

    Logger(String category, LogLevel threshold, LogManager manager) {
        this.category = category;
        this.threshold = threshold;
        this.manager = manager;
    }

    void updateThreshold(LogLevel newThreshold) { this.threshold = newThreshold; }

    public boolean isEnabled(LogLevel lvl) {
        return lvl.intValue() >= threshold.intValue();
    }

    // String message variants
    public void trace(String msg) { log(LogLevel.TRACE, msg, null); }
    public void debug(String msg) { log(LogLevel.DEBUG, msg, null); }
    public void info(String msg)  { log(LogLevel.INFO,  msg, null); }
    public void warn(String msg)  { log(LogLevel.WARN,  msg, null); }
    public void error(String msg) { log(LogLevel.ERROR, msg, null); }
    public void fatal(String msg) { log(LogLevel.FATAL, msg, null); }

    // Throwable variants
    public void warn(String msg, Throwable t)  { log(LogLevel.WARN,  msg, t); }
    public void error(String msg, Throwable t) { log(LogLevel.ERROR, msg, t); }
    public void fatal(String msg, Throwable t) { log(LogLevel.FATAL, msg, t); }

    // Lazy supplier variant for expensive message construction
    public void debug(Supplier<String> msgSupplier) {
        if (!isEnabled(LogLevel.DEBUG)) return;
        log(LogLevel.DEBUG, safeGet(msgSupplier), null);
    }
    public void trace(Supplier<String> msgSupplier) {
        if (!isEnabled(LogLevel.TRACE)) return;
        log(LogLevel.TRACE, safeGet(msgSupplier), null);
    }

    // Format-style naive implementation - keeps zero allocations for simple path
    public void info(String fmt, Object... args) {
        if (!isEnabled(LogLevel.INFO)) return;
        log(LogLevel.INFO, format(fmt, args), null);
    }

    private static String safeGet(Supplier<String> s) {
        try { return s.get(); } catch (Throwable t) { return "<exception in supplier: " + t + ">"; }
    }

    private static String format(String fmt, Object... args) {
        if (args == null || args.length == 0) return fmt;
        // simple replacement of {} with args — not a full formatter, but fast and predictable
        StringBuilder sb = new StringBuilder(fmt.length() + args.length * 8);
        int argIndex = 0;
        for (int i = 0; i < fmt.length(); ) {
            int j = fmt.indexOf("{}", i);
            if (j < 0) { sb.append(fmt, i, fmt.length()); break; }
            sb.append(fmt, i, j);
            if (argIndex < args.length) sb.append(String.valueOf(args[argIndex++]));
            else sb.append("{}");
            i = j + 2;
        }
        // append leftover args
        if (argIndex < args.length) {
            sb.append(" [");
            for (int k = argIndex; k < args.length; ++k) {
                if (k > argIndex) sb.append(", ");
                sb.append(String.valueOf(args[k]));
            }
            sb.append(']');
        }
        return sb.toString();
    }

    private void log(LogLevel level, String message, Throwable thrown) {
        Objects.requireNonNull(level, "level");
        if (!isEnabled(level)) return;
        LogRecord lr = new LogRecord(category, level, message, thrown);
        manager.enqueue(lr);
    }
}
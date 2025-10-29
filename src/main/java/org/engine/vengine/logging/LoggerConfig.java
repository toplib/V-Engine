
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


import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public final class LoggerConfig {
    // default config
    private volatile LogLevel level = LogLevel.INFO;
    private final List<Appender> appenders = new ArrayList<>();
    private volatile boolean async = true;
    private volatile int queueCapacity = 16384; // engine default

    public LoggerConfig() {}

    public LogLevel getLevel() { return level; }
    public LoggerConfig setLevel(LogLevel level) { this.level = level; return this; }

    public List<Appender> getAppenders() { return appenders; }
    public LoggerConfig addAppender(Appender a) { appenders.add(a); return this; }

    public boolean isAsync() { return async; }
    public LoggerConfig setAsync(boolean async) { this.async = async; return this; }

    public int getQueueCapacity() { return queueCapacity; }
    public LoggerConfig setQueueCapacity(int capacity) { this.queueCapacity = Math.max(256, capacity); return this; }

    // helper to add console quickly
    public LoggerConfig addConsole() { appenders.add(new ConsoleAppender()); return this; }

    // helper to add rolling file quickly
    public LoggerConfig addRollingFile(String path, long maxBytes, int maxIndex) {
        try {
            appenders.add(new RollingFileAppender(Paths.get(path).toString(), maxBytes, maxIndex));
        } catch (Exception e) {
            // fallback to console if file can't be created
            System.err.println("Failed to create rolling file appender: " + e.getMessage());
            appenders.add(new ConsoleAppender());
        }
        return this;
    }

}
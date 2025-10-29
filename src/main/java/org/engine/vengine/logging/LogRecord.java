
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

import java.time.Instant;

public final class LogRecord {
    public final Instant timestamp;
    public final long threadId;
    public final String threadName;
    public final String category;
    public final LogLevel level;
    public final String message;
    public final Throwable thrown;

    public LogRecord(String category, LogLevel level, String message, Throwable thrown) {
        this.timestamp = Instant.now();
        Thread t = Thread.currentThread();
        this.threadId = t.getId();
        this.threadName = t.getName();
        this.category = category;
        this.level = level;
        this.message = message;
        this.thrown = thrown;
    }
}
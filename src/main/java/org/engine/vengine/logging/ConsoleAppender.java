
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

import java.io.PrintStream;
import java.time.format.DateTimeFormatter;
import java.time.ZoneId;

public final class ConsoleAppender implements Appender {
    private final PrintStream out;
    private final DateTimeFormatter tsFmt;

    public ConsoleAppender() {
        this(System.out);
    }

    public ConsoleAppender(PrintStream out) {
        this.out = out;
        this.tsFmt = DateTimeFormatter.ISO_INSTANT.withZone(ZoneId.systemDefault());
    }

    @Override
    public void append(LogRecord r) {
        String line = format(r);
        out.println(line);
        if (r.thrown != null) {
            r.thrown.printStackTrace(out);
        }
    }

    private String format(LogRecord r) {
        return String.format("%s [%s] %s %-5s - %s",
                tsFmt.format(r.timestamp),
                r.threadName,
                r.category,
                r.level.name(),
                r.message);
    }

    @Override public void flush() { out.flush(); }
    @Override public void close() { /* nothing */ }
}
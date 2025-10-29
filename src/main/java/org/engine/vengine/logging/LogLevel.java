
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

public enum LogLevel {
    TRACE(10), DEBUG(20), INFO(30), WARN(40), ERROR(50), FATAL(60);

    private final int priority;

    LogLevel(int priority) { this.priority = priority; }

    public int intValue() { return priority; }

    public boolean isEnabled(LogLevel threshold) {
        return this.priority >= threshold.priority;
    }
}
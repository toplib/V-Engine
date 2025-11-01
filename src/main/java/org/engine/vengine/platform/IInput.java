
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

package org.engine.vengine.platform;

import org.joml.Vector2f;

public interface IInput {
    void setActiveWindow(long window);
    int getKeyState(int keyCode);
    boolean getKey(int keyCode);
    boolean getKeyDown(int keyCode);
    boolean getKeyUp(int keyCode);

    Vector2f getCursorPosition();
    Vector2f getScrollDelta();

    boolean getMouseButton(int button);
    boolean getMouseButtonDown(int button);
    boolean getMouseButtonUp(int button);

    String getClipboardString();
    void setClipboardString(String clipboardString);
}

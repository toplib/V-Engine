
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

package org.engine.vengine.platform.windows;

import org.engine.vengine.platform.IInput;
import org.joml.Vector2f;
import org.lwjgl.glfw.GLFW;

import java.util.HashMap;
import java.util.Map;

import static org.lwjgl.glfw.GLFW.*;

public class Input implements IInput {
    private static long windowId = 0;
    private static Map<Integer, Integer> keys = new HashMap<>();

    @Override
    public void setActiveWindow(long window) {
        windowId = window;
    }

    @Override
    public int getKeyState(int keyCode) {
        if(windowId == GLFW_FALSE)
            throw new IllegalStateException("Error while attempting to get key, " +
                    "window id is 0. Did you forgot set active window?");
        return glfwGetKey(windowId, keyCode);
    }

    @Override
    public boolean getKey(int keyCode) {
        int state = getKeyState(keyCode);
        return state == GLFW_PRESS;
    }

    @Override
    public boolean getKeyDown(int keyCode) {
        int currentState = getKeyState(keyCode);
        int prevState = keys.getOrDefault(keyCode, GLFW_RELEASE);

        boolean result = currentState == GLFW_PRESS && prevState == GLFW_RELEASE;
        keys.put(keyCode, currentState);
        return result;
    }

    @Override
    public boolean getKeyUp(int keyCode) {
        int currentState = getKeyState(keyCode);
        int prevState = keys.getOrDefault(keyCode, GLFW_RELEASE);

        boolean result = currentState == GLFW_RELEASE && prevState == GLFW_PRESS;
        keys.put(keyCode, currentState);
        return result;
    }


    /**
     * Get cursor position
     *
     * @return Vector2 with position
     */
    @Override
    public Vector2f getCursorPosition() {
        if(windowId == GLFW_FALSE)
            throw new IllegalStateException("Error while attempting to get key, " +
                    "window id is 0. Did you forgot set active window?");
        double[] x = new double[1];
        double[] y = new double[1];
        glfwGetCursorPos(windowId, x,y);
        return new Vector2f((float) x[0], (float) y[0]);
    }

    /**
     * kjgkfj
     *
     * @return null
     */
    @Override
    public Vector2f getScrollDelta() {
        if(windowId == GLFW_FALSE)
            throw new IllegalStateException("Error while attempting to get key, " +
                    "window id is 0. Did you forgot set active window?");
        double[] xOffset = new double[1];
        double[] yOffset = new double[1];
        //GLFW.
        //glfwGetScr()
        return new Vector2f((float) xOffset[0], (float) yOffset[0]);
    }

    @Override
    public boolean getMouseButton(int button) {
        if(windowId == GLFW_FALSE)
            throw new IllegalStateException("Error while attempting to get key, " +
                    "window id is 0. Did you forgot set active window?");
        int state = glfwGetKey(windowId, button);
        return state == GLFW_PRESS;
    }

    @Override
    public boolean getMouseButtonDown(int button) {
        if(windowId == GLFW_FALSE)
            throw new IllegalStateException("Error while attempting to get key, " +
                    "window id is 0. Did you forgot set active window?");
        int state = glfwGetKey(windowId, button);
        return state == GLFW_PRESS;
    }

    /**
     * Get some string from clipboard
     *
     * @param button Button id that will be checked
     * @return Button state
     */
    @Override
    public boolean getMouseButtonUp(int button) {
        if(windowId == GLFW_FALSE)
            throw new IllegalStateException("Error while attempting to get key, " +
                    "window id is 0. Did you forgot set active window?");
        int state = glfwGetKey(windowId, button);
        return state == GLFW_PRESS;
    }

    /**
     * Get some string from clipboard
     *
     * @return String from keyboard
     */
    @Override
    public String getClipboardString() {
        if(windowId == GLFW_FALSE)
            throw new IllegalStateException("Error while attempting to get key, " +
                    "window id is 0. Did you forgot set active window?");
        return glfwGetClipboardString(windowId);
    }

    /**
     * Copies some string in clipboard.
     *
     * @param clipboardString String that will be copied
     */
    @Override
    public void setClipboardString(String clipboardString) {
        if(windowId == GLFW_FALSE)
            throw new IllegalStateException("Error while attempting to get key, " +
                    "window id is 0. Did you forgot set active window?");
        glfwSetClipboardString(windowId, clipboardString);
    }
}

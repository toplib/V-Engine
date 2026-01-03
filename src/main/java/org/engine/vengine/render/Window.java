
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

package org.engine.vengine.render;

import org.lwjgl.glfw.*;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryStack;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.nio.IntBuffer;

import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.*;

public class Window {
    private static final Logger logger = LoggerFactory.getLogger("Window");

    private WindowData data;
    private long handle;
    public Window(WindowData data) {
        this.data = data;
    }

    public void init(){
        if ( !glfwInit() )
            throw new IllegalStateException("Unable to initialize GLFW");

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, data.isVisible() ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, data.isResizable() ? GLFW_TRUE : GLFW_FALSE);

        handle = glfwCreateWindow(data.getWidth(),
                data.getHeight(),
                data.getTitle(),
                NULL,
                NULL);
        if ( handle == NULL )
            throw new RuntimeException("Unable to create GLFW window");

        Size windowSize = getWindowSize(handle);
        Size screenSize = getScreenSize();

        glfwSetWindowPos(
                handle,
                (screenSize.getWidth() - windowSize.getWidth()) / 2,
                (screenSize.getHeight() - windowSize.getHeight()) / 2
        );

        glfwMakeContextCurrent(handle);
        swapInterval(1);
    }

    public void startRenderLoop(){
        GL.createCapabilities();
        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        while ( !glfwWindowShouldClose(handle) ) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glfwSwapBuffers(handle);
            glfwPollEvents();
        }
    }

    public void destroy(){
        glfwFreeCallbacks(handle);
        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    private void swapInterval(int interval){
        glfwSwapInterval(interval);
    }

    private Size getWindowSize(long window){
        try ( MemoryStack stack = stackPush() ) {
            IntBuffer pWidth = stack.mallocInt(1); // int*
            IntBuffer pHeight = stack.mallocInt(1); // int*

            // Get the window size passed to glfwCreateWindow
            glfwGetWindowSize(window, pWidth, pHeight);
            return new Size(pWidth.get(0), pHeight.get(0));
        }
    }
    private Size getScreenSize(){
        GLFWVidMode vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        return new Size(vidmode.width(),
                vidmode.height());
    }


}

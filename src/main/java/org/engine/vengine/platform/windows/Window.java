
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

import org.engine.vengine.platform.IWindow;
import org.engine.vengine.render.Renderer;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;



import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.glfw.GLFW.glfwTerminate;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.*;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;
import org.lwjgl.system.*;

import java.nio.IntBuffer;

public class Window implements IWindow {
    private static final Logger logger = LoggerFactory.getLogger(Window.class);

    private final int width;
    private final int height;
    private String title;
    private long windowId;
    public Window(){
        this.title = "te";
        this.width = 800;
        this.height = 600;

        if (!glfwInit()) {
            logger.error("OpenGL init Error, cannot initialize GLFW. Application will close soon.");
            System.exit(-1);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        windowId = glfwCreateWindow(width, height, title, NULL, NULL);
        if (windowId == NULL) {
            logger.error("Failed to create GLFW window");
            System.exit(-1);
        }
        glfwMakeContextCurrent(windowId);
        GL.createCapabilities();
        glViewport(0, 0, width, height);
        centerWindow();
        glfwShowWindow(windowId);
        glEnable(GL_DEPTH_TEST);
        while(!glfwWindowShouldClose(windowId) && !Thread.currentThread().isInterrupted()){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0f, 0f, 0f, 1.0f);
            glfwSwapBuffers(windowId);
            glfwPollEvents();


        }
    }
    public void centerWindow(){
        try (MemoryStack stack = stackPush()) {
            IntBuffer pWidth = stack.mallocInt(1);
            IntBuffer pHeight = stack.mallocInt(1);
            glfwGetWindowSize(windowId, pWidth, pHeight);
            GLFWVidMode videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            assert videoMode != null;
            glfwSetWindowPos(
                    windowId,
                    (videoMode.width() - pWidth.get(0)) / 2,
                    (videoMode.height() - pHeight.get(0)) / 2
            );
        }
    }

    @Override
    public long getWindowId() {
        return 0;
    }

    @Override
    public void show() {

    }

    @Override
    public void hide() {

    }

    @Override
    public Renderer getRenderer() {
        return null;
    }
}

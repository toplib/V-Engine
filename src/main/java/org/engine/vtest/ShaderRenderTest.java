
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

package org.engine.vtest;

import org.engine.vengine.filesystem.DefaultResourceLoader;
import org.engine.vengine.filesystem.FileManager;
import org.engine.vengine.shader.Shader;
import org.lwjgl.Version;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryUtil;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL30.*;
import static org.lwjgl.system.MemoryUtil.NULL;

/**
 * Minimal test/demo class to verify Shader and basic rendering using LWJGL.
 *
 * Creates a window, initializes OpenGL context, compiles shaders,
 * draws a triangle, and then cleans up.
 */
public class ShaderRenderTest {

    private long window;
    private Shader shader;
    private int vaoId;
    private int vboId;

    public static void main(String[] args) {
        new ShaderRenderTest().run();
    }

    public void run() {
        System.out.println("LWJGL " + Version.getVersion() + " initialized.");

        init();
        loop();
        cleanup();
    }

    /** Initialize GLFW window and OpenGL context */
    private void init() {
        GLFWErrorCallback.createPrint(System.err).set();

        if (!glfwInit()) {
            throw new IllegalStateException("Unable to initialize GLFW");
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(800, 600, "Shader Test", NULL, NULL);
        if (window == NULL) {
            throw new RuntimeException("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        glfwShowWindow(window);

        // Initialize OpenGL bindings
        GL.createCapabilities();

        // Setup shader
        FileManager fileManager = new FileManager();
        DefaultResourceLoader defaultLoader = new DefaultResourceLoader();
        fileManager.registerLoader(defaultLoader, "vert", "frag", "vs", "fs", "png", "jpg", "obj", "txt", "raw");
        shader = new Shader(fileManager)
                .attach("shaders/basic.vert")
                .attach("shaders/basic.frag")
                .link();

        // Setup geometry (simple triangle)
        float[] vertices = {
                // positions (x, y, z)
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f,  0.5f, 0.0f
        };

        vaoId = glGenVertexArrays();
        glBindVertexArray(vaoId);

        vboId = glGenBuffers();
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    /** Main rendering loop */
    private void loop() {
        double time = 0;
        double lastTime = glfwGetTime();
        while (!glfwWindowShouldClose(window)) {
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastTime;
            time += deltaTime;
            lastTime = currentTime;

            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            shader.bind();
            shader.setUniform("u_time", (float) time);
            shader.setUniform("u_color",
                    0.5f + 0.5f * (float) Math.sin(time / 1),
                    0.5f + 0.5f * (float) Math.sin(time / 2),
                    0.5f + 0.5f * (float) Math.sin(time / 3),
                    1.0f);

            glBindVertexArray(vaoId);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            shader.unbind();

            glfwSwapBuffers(window);
        }
    }


    /** Cleanup all GPU resources and GLFW */
    private void cleanup() {
        shader.cleanup();

        glDeleteVertexArrays(vaoId);
        glDeleteBuffers(vboId);

        glfwDestroyWindow(window);
        glfwTerminate();
        glfwSetErrorCallback(null).free();
    }
}

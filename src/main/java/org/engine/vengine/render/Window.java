
/*
 * MIT License
 *
 *  Copyright (c) 2026 toplib
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

package org.engine.vengine.render;

import org.engine.vengine.Scene;
import org.engine.vengine.TestObject;
import org.engine.vengine.mesh.Mesh;
import org.engine.vengine.mesh.MeshData;
import org.engine.vengine.render.material.Material;
import org.engine.vengine.render.renderer.MeshRenderer;
import org.engine.vengine.render.renderer.Renderer;
import org.engine.vengine.render.shader.Shader;
import org.engine.vengine.render.shader.ShaderProgram;
import org.engine.vengine.texture.Image;
import org.engine.vengine.texture.Texture;
import org.engine.vengine.util.Size;
import org.joml.Matrix4f;
import org.joml.Vector3f;
import org.joml.Vector4f;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryStack;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.nio.IntBuffer;

import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.system.MemoryStack.stackPush;
import static org.lwjgl.system.MemoryUtil.NULL;

public class Window {

    private static final Logger logger = LoggerFactory.getLogger("Window");

    private final WindowData data;
    private long handle;

    public Window(WindowData data) {
        this.data = data;
    }

    public void init() {
        if (!glfwInit())
            throw new IllegalStateException("Unable to initialize GLFW");

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, data.isVisible() ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, data.isResizable() ? GLFW_TRUE : GLFW_FALSE);

        handle = glfwCreateWindow(
                data.getWidth(),
                data.getHeight(),
                data.getTitle(),
                NULL,
                NULL
        );

        if (handle == NULL)
            throw new RuntimeException("Unable to create GLFW window");

        Size windowSize = getWindowSize(handle);
        Size screenSize = getScreenSize();

        glfwSetWindowPos(
                handle,
                (screenSize.getWidth() - windowSize.getWidth()) / 2,
                (screenSize.getHeight() - windowSize.getHeight()) / 2
        );

        glfwMakeContextCurrent(handle);
        glfwSwapInterval(1);
    }

    // global variables for camera
    private float yaw = -90.0f;
    private float pitch = 0.0f;
    private float fov = 80.0f;

    private float lastX = 400.0f;
    private float lastY = 300.0f;
    private boolean firstMouse = true;

    private Vector3f cameraPos = new Vector3f(0.0f, 0.0f, 3.0f);
    private Vector3f cameraFront = new Vector3f(0.0f, 0.0f, -1.0f);
    private final Vector3f cameraUp = new Vector3f(0.0f, 1.0f, 0.0f);

    public void startRenderLoop() {
        GL.createCapabilities();

        glEnable(GL_DEPTH_TEST);
        glClearColor(0f, 0f, 0f, 1f);

        Matrix4f model = new Matrix4f();
        Matrix4f view = new Matrix4f();
        Matrix4f projection = new Matrix4f()
                .perspective((float) Math.toRadians(fov), 800f / 600f, 0.1f, 100f);

        TestObject to = new TestObject();

        Scene scene = new Scene();
        scene.renderers.add(to.getMeshRenderer());

        Renderer renderer = new Renderer();

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        // hide and capture cursor
        glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // mouse callback
        glfwSetCursorPosCallback(handle, (window, xpos, ypos) -> {
            if (firstMouse) {
                lastX = (float) xpos;
                lastY = (float) ypos;
                firstMouse = false;
            }

            float xoffset = (float) xpos - lastX;
            float yoffset = lastY - (float) ypos; // reversed since y-coordinates range bottom-top
            lastX = (float) xpos;
            lastY = (float) ypos;

            float sensitivity = 0.1f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;
            pitch += yoffset;

            // constrain pitch
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            // update camera front vector
            cameraFront.x = (float) (Math.cos(Math.toRadians(yaw)) * Math.cos(Math.toRadians(pitch)));
            cameraFront.y = (float) Math.sin(Math.toRadians(pitch));
            cameraFront.z = (float) (Math.sin(Math.toRadians(yaw)) * Math.cos(Math.toRadians(pitch)));
            cameraFront.normalize();
        });

        // scroll callback for zoom
        glfwSetScrollCallback(handle, (window, xoffset, yoffset) -> {
            fov -= (float) yoffset;
            if (fov < 1.0f) fov = 1.0f;
            if (fov > 45.0f) fov = 45.0f;
        });

        while (!glfwWindowShouldClose(handle)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float currentFrame = (float) glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            float cameraSpeed = 2.5f * deltaTime;

            // keyboard movement
            if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS) {
                cameraPos.fma(cameraSpeed, cameraFront); // equivalent to add(cameraFront * speed)
            }
            if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS) {
                cameraPos.fma(-cameraSpeed, cameraFront);
            }
            if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS) {
                Vector3f right = new Vector3f(cameraFront).cross(cameraUp).normalize().mul(cameraSpeed);
                cameraPos.sub(right);
            }
            if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS) {
                Vector3f right = new Vector3f(cameraFront).cross(cameraUp).normalize().mul(cameraSpeed);
                cameraPos.add(right);
            }

            // update view and projection
            Vector3f cameraTarget = new Vector3f(cameraPos).add(cameraFront);
            view.identity().lookAt(cameraPos, cameraTarget, cameraUp);
            projection.identity().perspective((float) Math.toRadians(fov), 800f / 600f, 0.1f, 100f);

            to.getShaderProgram().bind();
            to.getShaderProgram().setMatrix4f("model", model);
            to.getShaderProgram().setMatrix4f("view", view);
            to.getShaderProgram().setMatrix4f("projection", projection);

            glActiveTexture(GL_TEXTURE0);
            to.getTexture().bind();

            renderer.render(scene);

            glfwSwapBuffers(handle);
            glfwPollEvents();
        }
    }




    public void destroy() {
        glfwFreeCallbacks(handle);
        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    private Size getWindowSize(long window) {
        try (MemoryStack stack = stackPush()) {
            IntBuffer w = stack.mallocInt(1);
            IntBuffer h = stack.mallocInt(1);
            glfwGetWindowSize(window, w, h);
            return new Size(w.get(0), h.get(0));
        }
    }

    private Size getScreenSize() {
        GLFWVidMode vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        return new Size(vidmode.width(), vidmode.height());
    }
}
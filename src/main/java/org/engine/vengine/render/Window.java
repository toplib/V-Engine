
/*
 * MIT License  Copyright (c) 2026 toplib  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

package org.engine.vengine.render;

import org.engine.vengine.Scene;
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

    public void startRenderLoop() {
        GL.createCapabilities();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0f, 0f, 0f, 1f);

        // ===== SHADERS =====
        Shader vertexShader = new Shader(GL_VERTEX_SHADER);
        vertexShader.source(
                "#version 330 core\n" +
                        "layout (location = 0) in vec3 aPos;\n" +
                        "layout (location = 1) in vec2 aTexCoord;\n" +
                        "out vec2 TexCoord;\n" +
                        "void main() {\n" +
                        "    gl_Position = vec4(aPos, 1.0);\n" +
                        "    TexCoord = aTexCoord;\n" +
                        "}\n"
        );
        vertexShader.compile();

        Shader fragmentShader = new Shader(GL_FRAGMENT_SHADER);
        fragmentShader.source(
                "#version 330 core\n" +
                        "out vec4 FragColor;\n" +
                        "in vec2 TexCoord;\n" +
                        "uniform sampler2D ourTexture;\n" +
                        "void main() {\n" +
                        "    FragColor = texture(ourTexture, TexCoord);\n" +
                        "}\n"
        );
        fragmentShader.compile();

        ShaderProgram shaderProgram = new ShaderProgram();
        shaderProgram.attach(vertexShader);
        shaderProgram.attach(fragmentShader);
        shaderProgram.link();
        shaderProgram.bind();
        shaderProgram.setInt("ourTexture", 0);

        Material material = new Material();
        material.shader = shaderProgram;

        // ===== TEXTURE =====
        Image image = new Image("./resources/texture.png");
        Texture texture = new Texture(image);

        // ===== MESH =====
        float[] vertices = {
                // positions         // texture coords
                0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
                0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
                -0.5f,  0.5f, 0.0f,   0.0f, 1.0f
        };

        int[] indices = {
                0, 1, 3,
                1, 2, 3
        };

        Mesh mesh = new Mesh(new MeshData(vertices, indices));
        MeshRenderer meshRenderer = new MeshRenderer(mesh, material);

        Scene scene = new Scene();
        scene.renderers.add(meshRenderer);

        Renderer renderer = new Renderer();

        // ===== LOOP =====
        while (!glfwWindowShouldClose(handle)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            texture.bind();
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

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

package org.engine.vengine;

import org.engine.vengine.mesh.Mesh;
import org.engine.vengine.mesh.MeshData;
import org.engine.vengine.render.material.Material;
import org.engine.vengine.render.renderer.MeshRenderer;
import org.engine.vengine.render.shader.Shader;
import org.engine.vengine.render.shader.ShaderProgram;
import org.engine.vengine.texture.Image;
import org.engine.vengine.texture.Texture;

import static org.lwjgl.opengl.GL20.GL_FRAGMENT_SHADER;
import static org.lwjgl.opengl.GL20.GL_VERTEX_SHADER;

public class TestObject {
    private MeshRenderer ms;
    private Mesh mesh;
    private ShaderProgram shaderProgram;
    private Shader vertex;
    private Shader fragment;
    private Image image;
    private Texture texture;

    public TestObject(){
        this.vertex = new Shader(GL_VERTEX_SHADER);
        this.vertex.source(
                "#version 330 core\n" +
                        "layout (location = 0) in vec3 aPos;\n" +
                        "layout (location = 1) in vec2 aTex;\n" +
                        "\n" +
                        "out vec2 vTex;\n" +
                        "uniform mat4 model;\n" +
                        "uniform mat4 view;\n" +
                        "uniform mat4 projection;\n" +
                        "\n" +
                        "void main() {\n" +
                        "    vTex = aTex;\n" +
                        "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n" +
                        "}\n"
        );
        this.vertex.compile();

        this.fragment = new Shader(GL_FRAGMENT_SHADER);
        this.fragment.source(
                "#version 330 core\n" +
                        "out vec4 FragColor;\n" +
                        "\n" +
                        "in vec2 vTex;\n" +
                        "uniform sampler2D ourTexture;\n" +
                        "\n" +
                        "void main() {\n" +
                        "    FragColor = texture(ourTexture, vTex);\n" +
                        "}\n"
        );
        this.fragment.compile();

        this.shaderProgram = new ShaderProgram();
        this.shaderProgram.attach(vertex);
        this.shaderProgram.attach(fragment);
        this.shaderProgram.link();
        this.shaderProgram.bind();
        this.shaderProgram.setInt("ourTexture", 0);

        Material material = new Material();
        material.shader = shaderProgram;

        // ===== TEXTURE =====
        this.image = new Image("./resources/texture.png");
        this.texture = new Texture(image);

        // ===== MESH =====
        float[] vertices = {

                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        int[] indices = {
                0,  1,  2,   3,  4,  5,
                6,  7,  8,   9, 10, 11,
                12, 13, 14,  15, 16, 17,
                18, 19, 20,  21, 22, 23,
                24, 25, 26,  27, 28, 29,
                30, 31, 32,  33, 34, 35
        };



        this.mesh = new Mesh(new MeshData(vertices, indices));
        this.ms = new MeshRenderer(mesh, material);
    }

    public MeshRenderer getMeshRenderer(){
        return ms;
    }

    public Mesh getMesh() {
        return mesh;
    }

    public ShaderProgram getShaderProgram() {
        return shaderProgram;
    }

    public Shader getVertex() {
        return vertex;
    }

    public Shader getFragment() {
        return fragment;
    }

    public Image getImage() {
        return image;
    }

    public Texture getTexture() {
        return texture;
    }
}

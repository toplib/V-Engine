
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

package org.engine.vengine.util;

import org.engine.vengine.render.shader.Shader;
import org.engine.vengine.render.shader.ShaderProgram;

import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL30.*;

public class StupidObjectThatRenders {

    private final int vao;
    ShaderProgram shaderProgram;
    private final float[] vertices = {
            0.5f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
    };

    private final int[] indices = {
            0, 1, 3,
            1, 2, 3
    };

    public StupidObjectThatRenders() {

        vao = glGenVertexArrays();
        glBindVertexArray(vao);

        int vbo = glGenBuffers();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

        int ebo = glGenBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * Float.BYTES, 0);
        glEnableVertexAttribArray(0);

        Shader vertex = new Shader(GL_VERTEX_SHADER);
        vertex.source("#version 330 core\n" +
                "layout (location = 0) in vec3 aPos;\n" +
                "void main() {\n" +
                "    gl_Position = vec4(aPos, 1.0);\n" +
                "}");
        vertex.compile();

        Shader fragment = new Shader(GL_FRAGMENT_SHADER);
        fragment.source("#version 330 core\n" +
                "out vec4 FragColor;\n" +
                "void main() {\n" +
                "    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n" +
                "}");
        fragment.compile();

        shaderProgram = new ShaderProgram();
        shaderProgram.attach(vertex);
        shaderProgram.attach(fragment);
        shaderProgram.link();

        vertex.delete();
        fragment.delete();
        glBindVertexArray(0);
    }

    public void render() {
        shaderProgram.bind();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    public void delete() {
        shaderProgram.delete();
        glDeleteVertexArrays(vao);
    }
}

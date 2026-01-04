
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

package org.engine.vengine.render.shader;

import static org.lwjgl.opengl.GL20.*;
import org.joml.Matrix4f;
import org.joml.Vector3f;
import org.lwjgl.BufferUtils;

import java.nio.FloatBuffer;

public class ShaderProgram {
    private static final FloatBuffer MAT4_BUFFER =
            BufferUtils.createFloatBuffer(16);

    private int program;

    public ShaderProgram(){
        program = glCreateProgram();
    }

    public void attach(Shader shader){
        glAttachShader(program, shader.getId());
    }

    public void link() {
        glLinkProgram(program);
        if (glGetProgrami(program, GL_LINK_STATUS) != GL_TRUE) {
            throw new RuntimeException(glGetProgramInfoLog(program));
        }
    }
    public void bind() {
        glUseProgram(program);
    }

    public void unbind() {
        glUseProgram(0);
    }

    public void delete(){
        glDeleteProgram(program);
    }

    public void setInt(String name, int value) {
        int loc = getUniformLocation(name);
        glUniform1i(loc, value);
    }

    public void setFloat(String name, float value) {
        int loc = getUniformLocation(name);
        glUniform1f(loc, value);
    }

    public void setVector3f(String name, Vector3f value) {
        int loc = getUniformLocation(name);
        glUniform3f(loc, value.x, value.y, value.z);
    }


    public void setMatrix4f(String name, Matrix4f matrix) {
        int loc = getUniformLocation(name);
        MAT4_BUFFER.clear();
        matrix.get(MAT4_BUFFER);
        glUniformMatrix4fv(loc, false, MAT4_BUFFER);
    }

    private int getUniformLocation(String name){
        int loc = glGetUniformLocation(program, name);
        if (loc == -1)
            throw new RuntimeException("Uniform not found: " + name);
        return loc;
    }
}

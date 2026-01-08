
/*
 * MIT License  Copyright (c) 2026 toplib  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

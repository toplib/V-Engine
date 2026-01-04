
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

public class Shader {
    private int id;

    public Shader(int type){
        this.id = glCreateShader(type);
    }
    public static Shader createShader(int type, String source) {
        Shader shader = new Shader(type);
        shader.source(source);
        shader.compile();

        return shader;
    }

    public void source(String source){
        glShaderSource(id, source);
    }

    public void compile(){
        glCompileShader(id);
        int status = glGetShaderi(id, GL_COMPILE_STATUS);
        if (status != GL_TRUE) {
            throw new RuntimeException(glGetShaderInfoLog(id));
        }
    }
    public int check(){
        return glGetShaderi(id, GL_COMPILE_STATUS);
    }
    public int getId(){
        return this.id;
    }

    public void delete() {
        glDeleteShader(id);
    }
}

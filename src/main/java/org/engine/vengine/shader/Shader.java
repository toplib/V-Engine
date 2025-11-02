
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

package org.engine.vengine.shader;

import org.engine.vengine.filesystem.FileManager;
import org.engine.vengine.filesystem.Resource;
import org.engine.vengine.filesystem.ResourceType;
import org.lwjgl.opengl.GL20;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.nio.file.Files;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.Map;

import static org.lwjgl.opengl.GL20.*;

/**
 * Represents a full OpenGL shader program that can compile, link, and manage multiple shader stages.
 *
 * Designed with extensibility and clean architecture in mind.
 * You can safely extend shader functionality (e.g. geometry, compute shaders) by adding new ResourceType → GL type mappings.
 */
public class Shader {

    private static final Logger logger = LoggerFactory.getLogger(Shader.class);

    /** Handles shader file access. */
    private final FileManager fileManager;

    /** OpenGL program ID. */
    private final int programId;

    /** Maps each shader type to its OpenGL shader object ID. */
    private final Map<ResourceType, Integer> shaderStages = new EnumMap<>(ResourceType.class);

    /** Caches uniform locations to avoid repeated glGetUniformLocation calls. */
    private final Map<String, Integer> uniformCache = new HashMap<>();

    /**
     * Creates a new shader program and prepares it for use.
     *
     * @param fileManager the FileManager used to load shader source files.
     */
    public Shader(FileManager fileManager) {
        this.fileManager = fileManager;
        this.programId = glCreateProgram();

        if (programId == 0) {
            throw new IllegalStateException("Failed to create OpenGL shader program.");
        }
    }

    /**
     * Loads, compiles, and attaches a shader stage from file.
     * Automatically determines shader type via {@link ResourceType}.
     *
     * @param path shader source file path
     * @return this shader (for chaining)
     */
    public Shader attach(String path) {
        Resource resource = fileManager.loadResource(path);
        ResourceType type = resource.getType();

        int glShaderType = ShaderTypeMapper.map(type);
        String source = readShaderSource(resource);

        int shaderId = compileShader(glShaderType, source, path);
        glAttachShader(programId, shaderId);

        shaderStages.put(type, shaderId);
        logger.info("Attached {} shader: {}", type, path);
        return this;
    }

    /**
     * Links all attached shaders into one program.
     * Deletes intermediate shader objects after linking.
     *
     * @return this shader (for chaining)
     */
    public Shader link() {
        glLinkProgram(programId);

        if (glGetProgrami(programId, GL_LINK_STATUS) == GL_FALSE) {
            String log = glGetProgramInfoLog(programId);
            logger.error("Shader program linking failed: {}", log);
            throw new RuntimeException("Shader linking failed:\n" + log);
        }

        // Free shader objects after successful linking
        shaderStages.values().forEach(GL20::glDeleteShader);
        shaderStages.clear();

        logger.info("Shader program linked successfully (ID = {}).", programId);
        return this;
    }

    /** Activates this shader program for rendering. */
    public void bind() {
        glUseProgram(programId);
    }

    /** Deactivates the currently active shader program. */
    public void unbind() {
        glUseProgram(0);
    }

    /** Deletes this shader program and releases all GPU resources. */
    public void cleanup() {
        unbind();
        if (programId != 0) {
            glDeleteProgram(programId);
            logger.info("Shader program (ID = {}) deleted.", programId);
        }
    }

    /** @return the OpenGL program ID */
    public int getProgramId() {
        return programId;
    }


    private int getUniformLocation(String name) {
        return uniformCache.computeIfAbsent(name, uniform -> {
            int location = glGetUniformLocation(programId, uniform);
            if (location < 0) {
                logger.warn("Uniform '{}' not found in shader (ID = {}).", uniform, programId);
            }
            return location;
        });
    }

    /** Sets a float uniform value. */
    public void setUniform(String name, float value) {
        glUniform1f(getUniformLocation(name), value);
    }

    /** Sets an integer uniform value. */
    public void setUniform(String name, int value) {
        glUniform1i(getUniformLocation(name), value);
    }

    /** Sets a vec2 uniform value. */
    public void setUniform(String name, float x, float y) {
        glUniform2f(getUniformLocation(name), x, y);
    }

    /** Sets a vec3 uniform value. */
    public void setUniform(String name, float x, float y, float z) {
        glUniform3f(getUniformLocation(name), x, y, z);
    }

    /** Sets a vec4 uniform value. */
    public void setUniform(String name, float x, float y, float z, float w) {
        glUniform4f(getUniformLocation(name), x, y, z, w);
    }

    /** Sets a 4x4 matrix uniform value (column-major order). */
    public void setUniformMat4(String name, float[] matrix) {
        glUniformMatrix4fv(getUniformLocation(name), false, matrix);
    }

    /** Reads shader source code from a resource file. */
    private String readShaderSource(Resource resource) {
        try {
            return Files.readString(resource.getFile().toPath());
        } catch (IOException e) {
            throw new RuntimeException("Failed to read shader source: " + resource.getFile(), e);
        }
    }

    /** Compiles a shader stage and returns its OpenGL ID. */
    private int compileShader(int glShaderType, String source, String path) {
        int shaderId = glCreateShader(glShaderType);
        if (shaderId == 0) {
            throw new IllegalStateException("Failed to create shader: " + path);
        }

        glShaderSource(shaderId, source);
        glCompileShader(shaderId);

        if (glGetShaderi(shaderId, GL_COMPILE_STATUS) == GL_FALSE) {
            String log = glGetShaderInfoLog(shaderId);
            logger.error("Shader compilation failed ({}): {}", path, log);
            throw new RuntimeException("Shader compilation error in " + path + ":\n" + log);
        }

        return shaderId;
    }

    private static final class ShaderTypeMapper {

        private static final Map<ResourceType, Integer> TYPE_MAP = new EnumMap<>(ResourceType.class);

        static {
            TYPE_MAP.put(ResourceType.VERTEX_SHADER, GL_VERTEX_SHADER);
            TYPE_MAP.put(ResourceType.FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
            // Extend here if needed:
            // TYPE_MAP.put(ResourceType.GEOMETRY_SHADER, GL_GEOMETRY_SHADER);
            // TYPE_MAP.put(ResourceType.COMPUTE_SHADER, GL_COMPUTE_SHADER);
        }

        private ShaderTypeMapper() {}

        static int map(ResourceType type) {
            Integer glType = TYPE_MAP.get(type);
            if (glType == null) {
                throw new IllegalArgumentException("Unsupported shader type: " + type);
            }
            return glType;
        }
    }
}

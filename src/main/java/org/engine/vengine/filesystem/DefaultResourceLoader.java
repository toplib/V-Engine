
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

package org.engine.vengine.filesystem;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.*;

/**
 * Default resource loader capable of recognizing shader, texture, model and raw files.
 * Supports .glsl files with embedded shader type directive (#type vertex / #type fragment).
 */
public final class DefaultResourceLoader implements IResourceLoader {

    private static final Set<String> VERT_EXT = new HashSet<>(Arrays.asList("vert", "vs", "vsh"));
    private static final Set<String> FRAG_EXT = new HashSet<>(Arrays.asList("frag", "fs", "fsh"));
    private static final Set<String> GLSL_EXT = new HashSet<>(Collections.singletonList("glsl"));
    private static final Set<String> TEXTURE_EXT = new HashSet<>(Arrays.asList("png", "jpg", "jpeg", "bmp", "tga"));
    private static final Set<String> OBJECT_EXT = new HashSet<>(Arrays.asList("obj", "fbx", "gltf", "glb"));
    private static final Set<String> RAW_EXT = new HashSet<>(Arrays.asList("txt", "raw", "bin", "dat"));

    @Override
    public Resource load(String path) throws FileSystemException {
        File file = new File(path);
        if (!file.exists() || !file.isFile()) {
            throw new FileSystemException("File does not exist or is not a file: " + path);
        }

        String ext = extensionOf(path);
        ResourceType type = detectType(ext, file);

        // Just ensure file is readable; in real engine we’d parse or upload it.
        try {
            Files.readAllBytes(file.toPath());
        } catch (IOException e) {
            throw new FileSystemException("Failed to read file: " + path, e);
        }

        return new Resource(type, file);
    }

    @Override
    public void unload(Resource resource) {
        // Nothing to free here by default
    }

    @Override
    public void cache(Resource resource) {
        // Optional caching logic, no-op by default
    }

    @Override
    public boolean supports(String path) {
        String ext = extensionOf(path);
        if (ext == null) return false;
        return VERT_EXT.contains(ext)
                || FRAG_EXT.contains(ext)
                || GLSL_EXT.contains(ext)
                || TEXTURE_EXT.contains(ext)
                || OBJECT_EXT.contains(ext)
                || RAW_EXT.contains(ext);
    }

    private static ResourceType detectType(String ext, File file) {
        if (ext == null) return ResourceType.RAW;

        if (VERT_EXT.contains(ext)) return ResourceType.VERTEX_SHADER;
        if (FRAG_EXT.contains(ext)) return ResourceType.FRAGMENT_SHADER;
        if (TEXTURE_EXT.contains(ext)) return ResourceType.TEXTURE;
        if (OBJECT_EXT.contains(ext)) return ResourceType.OBJECT;
        if (GLSL_EXT.contains(ext)) {
            // Try to detect shader type from file content
            try {
                List<String> lines = Files.readAllLines(file.toPath(), StandardCharsets.UTF_8);
                for (String line : lines) {
                    String lower = line.trim().toLowerCase(Locale.ROOT);
                    if (lower.contains("#type vertex")) return ResourceType.VERTEX_SHADER;
                    if (lower.contains("#type fragment")) return ResourceType.FRAGMENT_SHADER;
                }
            } catch (IOException ignored) {
                // fall back to raw if we can’t read
            }
            // If not found, default to RAW to avoid wrong assumption
            return ResourceType.RAW;
        }
        return ResourceType.RAW;
    }

    private static String extensionOf(String path) {
        if (path == null) return null;
        int dot = path.lastIndexOf('.');
        if (dot < 0 || dot == path.length() - 1) return null;
        return path.substring(dot + 1).toLowerCase(Locale.ROOT);
    }
}

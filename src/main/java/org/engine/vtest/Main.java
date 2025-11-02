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
import org.engine.vengine.filesystem.Resource;
import org.engine.vengine.platform.windows.Input;
import org.engine.vengine.platform.windows.Window;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import static org.lwjgl.glfw.GLFW.GLFW_KEY_A;

public class Main {
    private static final Logger logger = LoggerFactory.getLogger("VTest");

    public static void main(String[] args){
        logger.info("Creating new Application Instance");
        FileManager fileManager = new FileManager();
        DefaultResourceLoader defaultLoader = new DefaultResourceLoader();

        // register common extensions
        fileManager.registerLoader(defaultLoader, "vert", "frag", "vs", "fs", "png", "jpg", "obj", "txt", "raw");

        // load resource
        Resource res = fileManager.loadResource("resources/vertex.glsl");
        System.out.println("Loaded: " + res);

        // later, release
        fileManager.unloadResource(res);
        Window window = new Window();
    }
}

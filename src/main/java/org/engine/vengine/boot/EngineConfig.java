
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

package org.engine.vengine.boot;

import java.util.Locale;
import java.util.Objects;

/**
 * EngineConfig - V-Engine configuration
 * Using for initializing system parameters.
 */
public class EngineConfig {

    // --- Системные параметры ---
    private String appName = "V-Engine Application";
    private String version = "1.0.0";
    private boolean debug = false;
    private Locale locale = Locale.getDefault();

    // --- Графические параметры ---
    private int width = 1280;
    private int height = 720;
    private boolean fullscreen = false;
    private boolean vsync = true;
    private int fpsLimit = 144;
    private boolean resizable = true;
    private boolean antialiasing = true;
    private int samples = 4; // количество MSAA сэмплов

    // --- Рендеринг ---
    private boolean useVulkan = false; // если движок поддерживает Vulkan
    private boolean useOpenGL = true;
    private int glMajor = 4;
    private int glMinor = 6;

    // --- Путь к ресурсам ---
    private String assetsPath = "assets/";

    // --- Ввод ---
    private boolean enableGamepad = true;
    private boolean enableMouse = true;
    private boolean enableKeyboard = true;

    // --- Методы построения ---
    public EngineConfig setAppName(String appName) {
        this.appName = Objects.requireNonNull(appName);
        return this;
    }

    public EngineConfig setVersion(String version) {
        this.version = version;
        return this;
    }

    public EngineConfig setDebug(boolean debug) {
        this.debug = debug;
        return this;
    }

    public EngineConfig setResolution(int width, int height) {
        this.width = width;
        this.height = height;
        return this;
    }

    public EngineConfig setFullscreen(boolean fullscreen) {
        this.fullscreen = fullscreen;
        return this;
    }

    public EngineConfig setVsync(boolean vsync) {
        this.vsync = vsync;
        return this;
    }

    public EngineConfig setFpsLimit(int fpsLimit) {
        this.fpsLimit = fpsLimit;
        return this;
    }

    public EngineConfig setAntialiasing(boolean enabled, int samples) {
        this.antialiasing = enabled;
        this.samples = samples;
        return this;
    }

    public EngineConfig useVulkan(boolean enable) {
        this.useVulkan = enable;
        this.useOpenGL = !enable;
        return this;
    }

    public EngineConfig setAssetsPath(String assetsPath) {
        this.assetsPath = assetsPath;
        return this;
    }

    public EngineConfig setWidth(int width){
        this.width = width;
        return this;
    }

    public EngineConfig setHeight(int height){
        this.height = height;
        return this;
    }

    // --- Геттеры ---
    public String getAppName() { return appName; }
    public String getVersion() { return version; }
    public boolean isDebug() { return debug; }
    public int getWidth() { return width; }
    public int getHeight() { return height; }
    public boolean isFullscreen() { return fullscreen; }
    public boolean isVsync() { return vsync; }
    public int getFpsLimit() { return fpsLimit; }
    public boolean isAntialiasing() { return antialiasing; }
    public int getSamples() { return samples; }
    public boolean isUseVulkan() { return useVulkan; }
    public boolean isUseOpenGL() { return useOpenGL; }
    public String getAssetsPath() { return assetsPath; }

    @Override
    public String toString() {
        return "EngineConfig {" +
                "appName='" + appName + '\'' +
                ", version='" + version + '\'' +
                ", resolution=" + width + "x" + height +
                ", fullscreen=" + fullscreen +
                ", vsync=" + vsync +
                ", fpsLimit=" + fpsLimit +
                ", antialiasing=" + antialiasing +
                ", samples=" + samples +
                ", renderer=" + (useVulkan ? "Vulkan" : "OpenGL " + glMajor + "." + glMinor) +
                ", debug=" + debug +
                '}';
    }
}

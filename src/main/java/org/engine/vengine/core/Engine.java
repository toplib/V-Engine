package org.engine.vengine.core;

import org.engine.vengine.render.Renderer;
import org.engine.vengine.utils.Logger;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.opengl.GL;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;

public class Engine {
    private static final Logger logger = Logger.getLogger(Engine.class);
    private final EngineConfig config;
    private Window window;
    private Renderer renderer;
    private boolean running;
    public Engine() {
        this(EngineConfig.defaultConfig());
    }

    public Engine(EngineConfig config) {
        this.config = config;
        init();
    }

    private void init() {
        GLFWErrorCallback.createPrint(System.err).set();

        if (!glfwInit()) {
            throw new IllegalStateException("Unable to initialize GLFW");
        }

        window = new Window(config.getTitle(), config.getWidth(), config.getHeight());
        window.init();

        // Create OpenGL capabilities after making the context current and BEFORE
        // creating any GL objects (e.g., shaders in renderer.init()).
        GL.createCapabilities();

        renderer = new Renderer(window);
        renderer.init();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        logger.info("Engine initialized successfully");
    }

    public void start() {
        running = true;
        run();
    }

    private void run() {
        while (running && !window.shouldClose()) {
            update();
            render();
        }
        cleanup();
    }

    private void update() {
        window.update();
    }

    private void render() {
        renderer.render();
        window.swapBuffers();
    }

    private void cleanup() {
        renderer.cleanup();
        window.cleanup();
        glfwTerminate();
        glfwSetErrorCallback(null).free();
    }

    public void stop() {
        running = false;
    }
} 
package org.engine.vengine.render;

import org.engine.vengine.core.Window;
import org.engine.vengine.render.camera.Camera;
import org.engine.vengine.render.materials.Material;
import org.engine.vengine.render.materials.Texture2D;
import org.engine.vengine.render.shader.Shader;
import org.engine.vengine.utils.ResourceManager;
import org.joml.Matrix4f;
import org.joml.Vector3f;

import static org.lwjgl.opengl.GL11.*;

public class Renderer {
    private final Window window;
    private Camera camera;
    private Shader shader;
    private Material material;
    private Texture2D texture;
    private Matrix4f projection;

    public Renderer(Window window) {
        this.window = window;
    }

    public void init() {
        // Initialize camera
        camera = new Camera(
            new Vector3f(0.0f, 0.0f, 3.0f),
            new Vector3f(0.0f, 1.0f, 0.0f),
            -90.0f,
            0.0f
        );

        // Load shader
        shader = ResourceManager.loadShader("basic", "resources/vertex.glsl", "resources/fragment.glsl");

        // Load texture
        texture = ResourceManager.loadTexture("container", "resources/texture.png");

        // Create material
        material = new Material(shader);
        material.setTexture("ourTexture", texture);

        // Initialize projection matrix
        projection = new Matrix4f().perspective(
            (float) Math.toRadians(45.0f),
            (float) window.getWidth() / window.getHeight(),
            0.1f,
            100.0f
        );
    }

    public void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update view matrix
        Matrix4f view = camera.getViewMatrix();

        // Update shader uniforms
        shader.use();
        shader.setMatrix4f("projection", projection);
        shader.setMatrix4f("view", view);

        // Render objects
        material.apply();
        // TODO: Add object rendering here
    }

    public void cleanup() {
        shader.cleanup();
        texture.cleanup();
    }

    public Camera getCamera() {
        return camera;
    }
} 
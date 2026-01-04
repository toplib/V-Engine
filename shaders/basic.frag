#version 330 core
out vec4 FragColor;

uniform vec4 u_color;
uniform float u_time;

void main() {
    // Цвет будет слегка пульсировать
    float intensity = abs(sin(u_time));
    FragColor = vec4(u_color.rgb * intensity, 1.0);
}

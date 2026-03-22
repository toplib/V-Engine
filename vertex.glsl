#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int m_hasTexture;
flat out int o_m_hasTexture;
uniform vec4 m_color;
out vec4 o_m_color;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    o_m_color = m_color;
    o_m_hasTexture = m_hasTexture;
    Normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
}
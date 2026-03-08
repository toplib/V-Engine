#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 o_m_color;
flat in int o_m_hasTexture;

uniform sampler2D m_texture;

void main()
{
    if (o_m_hasTexture == 1) {
        FragColor = texture(m_texture, TexCoord);
    } else {
        FragColor = o_m_color;
    }
}
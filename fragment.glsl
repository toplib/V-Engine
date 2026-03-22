#version 330 core
out vec4 FragColor;

#define MAX_LIGHTS 10

struct Light {
    int m_type; // 0 - Point Light, 1 - Directional Light, 2 - Spot Light
    vec3 m_position;
    vec3 m_color;
};

struct Material {
    sampler2D m_albedo;
    float m_specularStrength;
};

uniform Light lights[MAX_LIGHTS];

in vec2 TexCoord;
in vec4 o_m_color;
flat in int o_m_hasTexture;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D m_texture;
uniform vec3 viewPos;
uniform float ambientStrength = 0.1f;
float specularStrength = 0.5;
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
void main()
{

    vec3 norm = normalize(Normal);
    vec3 lightPos = vec3(0.0f, 0.0f, 0.0f);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 result = vec4(1.0f);
    if (o_m_hasTexture == 1) {
        result = texture(m_texture, TexCoord);
    } else {
        result = o_m_color;
    }
    vec3 reflectDir = reflect(-lightDir, norm);

    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    result = result * o_m_color;
    vec4 ambdiff = vec4(ambient + diffuse + specular, 1.0f);


    result = result * ambdiff;
    FragColor = result;

}
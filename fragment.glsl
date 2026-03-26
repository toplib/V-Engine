#version 330 core
out vec4 FragColor;

#define MAX_LIGHTS 10

struct Light {
    int type; // 0 - Point Light, 1 - Directional Light, 2 - Spot Light
    vec3 position;
    vec3 color;
};

struct Material {
    sampler2D m_albedo;
    float m_specularStrength;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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

vec3 calculateLight(Light light){
    vec3 result = vec3(0.0f);
    if(light.type == 0) {
        vec3 lightDir = normalize(light.position - FragPos);
        vec3 norm = normalize(Normal);
        float modifier = 0.0f;
        modifier = normalize(abs(-length(FragPos - light.position)));
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * light.color * vec3(modifier);

        vec3 reflectDir = reflect(-lightDir, norm);

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 specular = vec3(0.0f);

        if(diff > 0.0f){
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            specular = specularStrength * spec * light.color;
        }
        result = diffuse + specular;
    }
    return result;
}

void main()
{
    // Base color calculation
    vec4 baseColor = vec4(1.0f);
    if (o_m_hasTexture == 1) {
        baseColor = texture(m_texture, TexCoord);
    } else {
        baseColor = o_m_color;
    }

    // Applying color modifier to base color
    baseColor = baseColor * o_m_color;

    vec3 lightPos = vec3(0.0f, 0.0f, 0.0f);
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightCalculation = vec3(0.0f);
    for(int i = 0; i < MAX_LIGHTS; i++){
        lightCalculation = lightCalculation + (calculateLight(lights[i]));
    }
    vec4 result = baseColor * vec4(ambient + lightCalculation, 1.0f);
    FragColor = result;
}


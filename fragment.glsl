#version 330 core
out vec4 FragColor;

#define MAX_LIGHTS 10

struct Light {
    int type; // 0 - Point Light, 1 - Directional Light, 2 - Spot Light
    vec4 position;
    vec4 color;
    float constant;
    float linear;
    float quadratic;
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
    if(light.type == 0) {  // Point Light
       vec3 lightPos = light.position.xyz;
       vec3 lightCol = light.color.xyz;

       vec3 lightDir = normalize(lightPos - FragPos);
       vec3 norm = normalize(Normal);

       float distance = length(lightPos - FragPos);
       float denom = light.constant + light.linear * distance + light.quadratic * (distance * distance);
       float attenuation = 1.0f / max(denom, 0.0001f);

       float diff = max(dot(norm, lightDir), 0.0);
       vec3 diffuse = diff * lightCol;

       vec3 reflectDir = reflect(-lightDir, norm);
       vec3 viewDir = normalize(viewPos - FragPos);
       vec3 specular = vec3(0.0f);

       if(diff > 0.0f) {
           float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
           specular = specularStrength * spec * lightCol;
       }

       result = (diffuse + specular) * attenuation;
    }
    return result;
}

void main()
{
    vec4 baseColor = vec4(1.0f);
    if (o_m_hasTexture == 1) {
        baseColor = texture(m_texture, TexCoord);
    } else {
        baseColor = o_m_color;
    }

    vec3 ambient = ambientStrength * lightColor;

    vec3 lightCalculation = vec3(0.0f);
    for(int i = 0; i < MAX_LIGHTS; i++){
        lightCalculation += calculateLight(lights[i]);
    }
    FragColor = baseColor * vec4(ambient + lightCalculation, 1.0f);
}
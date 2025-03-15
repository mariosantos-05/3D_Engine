#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D texture1;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

uniform vec3 viewPos;

#define NR_LIGHTS 4

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 color;
};

uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_LIGHTS];
uniform SpotLight spotLight;

// === LIGHTING CALCULATIONS ===

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir, vec2 texCoord) {
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = 0.1 * light.color;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    vec3 reflectDir = reflect(-lightDir, norm);
    float specStrength = texture(specularMap, texCoord).r;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specStrength * spec * light.color;

    return (ambient + diffuse + specular) * texture(texture1, texCoord).rgb;
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec2 texCoord) {
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));

    vec3 ambient = 0.1 * light.color;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    vec3 reflectDir = reflect(-lightDir, norm);
    float specStrength = texture(specularMap, texCoord).r;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specStrength * spec * light.color;

    return attenuation * (ambient + diffuse + specular) * texture(texture1, texCoord).rgb;
}

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec2 texCoord) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));

    vec3 ambient = 0.1 * light.color;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    vec3 reflectDir = reflect(-lightDir, norm);
    float specStrength = texture(specularMap, texCoord).r;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specStrength * spec * light.color;

    return attenuation * intensity * (ambient + diffuse + specular) * texture(texture1, texCoord).rgb;
}

void main()
{
    // Normal mapping
    vec3 norm = normalize(TBN * (texture(normalMap, TexCoord).rgb * 2.0 - 1.0));
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    result += CalcDirLight(dirLight, norm, viewDir, TexCoord);

    for (int i = 0; i < NR_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, TexCoord);
    }

    result += CalcSpotLight(spotLight, norm, FragPos, viewDir, TexCoord);

    FragColor = vec4(result, 1.0);
}

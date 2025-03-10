#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

uniform sampler2D texture1;    // Diffuse Texture
uniform sampler2D normalMap;   // Normal Map
uniform vec3 viewPos;

#define NR_LIGHTS 4  // Number of lights

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[NR_LIGHTS];

void main() {
    // Retrieve normal from normal map and convert it to [-1, 1] range
    vec3 normalMapValue = texture(normalMap, TexCoord).rgb;
    vec3 mappedNormal = normalize(normalMapValue * 2.0 - 1.0);

    // Transform normal from tangent space to world space
    vec3 norm = normalize(TBN * mappedNormal);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    for (int i = 0; i < NR_LIGHTS; i++) {
        // Ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lights[i].color;

        // Diffuse
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color;

        // Specular
        float specularStrength = 1.0;
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lights[i].color;

        // Combine lighting with texture color
        result += (ambient + diffuse + specular) * texture(texture1, TexCoord).rgb;
    }

    FragColor = vec4(result, 1.0);
}

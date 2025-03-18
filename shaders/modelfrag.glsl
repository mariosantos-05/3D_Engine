#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos = vec3(2.0, 4.0, 2.0);
uniform vec3 viewPos = vec3(0.0, 0.0, 3.0);

void main() {
    // Ambient
    vec3 ambient = vec3(0.1);

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.5);

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * vec3(0.2);

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}

#version 330 core
in vec3 FragColor;  // Input color from vertex shader
out vec4 FragColorOut;  // Output color to the framebuffer
void main() {
    FragColorOut = vec4(FragColor, 1.0);  // Set output color
}

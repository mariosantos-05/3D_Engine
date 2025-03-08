#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // Make sure to include GLAD before OpenGL
#include <string>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    // Constructor
    Shader(const char* vertexSource, const char* fragmentSource) {
        unsigned int vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
        unsigned int fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // Activate the shader
    void use() const { glUseProgram(ID); }


    void setMat4(const std::string& name, const float* value) const {
        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location == -1) {
            std::cerr << "Error: Uniform '" << name << "' not found in shader!" << std::endl;
            return;
        }
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}


    ~Shader() {
        glDeleteProgram(ID);
    }

private:
    unsigned int compileShader(const char* source, GLenum type) {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        checkCompileErrors(shader, (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"));
        return shader;
    }

    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
            }
        }
    }
};

#endif // SHADER_H

#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

class Material {
    public:
        GLuint diffuseMap;
        GLuint normalMap;
        float shininess;
    
        Material(GLuint diffuseMap, GLuint normalMap, float shininess = 32.0f)
            : diffuseMap(diffuseMap), normalMap(normalMap), shininess(shininess) {}
    
        void Bind(Shader& shader) const {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            shader.setInt("material.diffuse", 0);
    
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalMap);
            shader.setInt("material.normalMap", 1);
    
            shader.setFloat("material.shininess", shininess);
        }
    };
    

#endif

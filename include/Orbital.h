#ifndef ORBITAL_CAMERA_H
#define ORBITAL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OrbitalCamera {
public:
    OrbitalCamera(
        glm::vec3 target = glm::vec3(0.0f),
        float distance = 10.0f,
        float yaw = -90.0f,
        float pitch = 0.0f
    );

    glm::mat4 GetViewMatrix() const;
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);

    glm::vec3 GetCameraPosition() const;

private:
    glm::vec3 Target;
    float Distance;
    float Yaw;
    float Pitch;

    const float MinDistance = 1.0f;
    const float MaxDistance = 50.0f;
    const float Sensitivity = 0.3f;

    glm::vec3 CalculatePosition() const;
};

#endif

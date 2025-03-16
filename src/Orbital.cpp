#include "Orbital.h"
#include <glm/gtc/constants.hpp>

OrbitalCamera::OrbitalCamera(glm::vec3 target, float distance, float yaw, float pitch)
    : Target(target), Distance(distance), Yaw(yaw), Pitch(pitch) {}

glm::mat4 OrbitalCamera::GetViewMatrix() const {
    glm::vec3 position = CalculatePosition();
    return glm::lookAt(position, Target, glm::vec3(0, 1, 0));
}

glm::vec3 OrbitalCamera::CalculatePosition() const {
    float radPitch = glm::radians(Pitch);
    float radYaw = glm::radians(Yaw);

    float x = Distance * cos(radPitch) * cos(radYaw);
    float y = Distance * sin(radPitch);
    float z = Distance * cos(radPitch) * sin(radYaw);

    return Target + glm::vec3(-x, -y, -z);  // camera orbits *around* target
}

void OrbitalCamera::ProcessMouseMovement(float xoffset, float yoffset) {
    Yaw   += xoffset * Sensitivity;
    Pitch += yoffset * Sensitivity;

    // Clamp pitch to prevent flipping
    if (Pitch > 89.0f)  Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
}

void OrbitalCamera::ProcessMouseScroll(float yoffset) {
    Distance -= yoffset;
    if (Distance < MinDistance) Distance = MinDistance;
    if (Distance > MaxDistance) Distance = MaxDistance;
}

glm::vec3 OrbitalCamera::GetCameraPosition() const {
    return CalculatePosition();
}

#ifndef ORBITAL_CAMERA_H
#define ORBITAL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include "Window.h"

class OrbitalCamera {
public:
    OrbitalCamera(
        glm::vec3 target = glm::vec3(0.0f),
        float distance = 10.0f,
        float yaw = -90.0f,
        float pitch = 0.0f
    );

    void UpdateDeltaTime();
    void HandleKeyboard(const Uint8* state);
    void HandleMouseMotion(int xrel, int yrel);
    void HandleMouseScroll(float yoffset);
    void HandlePan(float xoffset, float yoffset);
    void HandleEvent(SDL_Event& event, Window& window, OrbitalCamera &camera, bool& running);


    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetCameraPosition() const;

private:
    glm::vec3 Target;
    float Distance;
    float Yaw;
    float Pitch;

    const float MinDistance = 1.0f;
    const float MaxDistance = 50.0f;
    const float Sensitivity = 0.1f;
    const float MoveSpeed = 5.0f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glm::vec3 CalculatePosition() const;
};

#endif

#include "Orbital.h"
#include <glm/gtc/constants.hpp>
#include <SDL2/SDL.h>

OrbitalCamera::OrbitalCamera(glm::vec3 target, float distance, float yaw, float pitch)
    : Target(target), Distance(distance), Yaw(yaw), Pitch(pitch) {}

void OrbitalCamera::UpdateDeltaTime() {
    float currentFrame = SDL_GetTicks() / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

glm::vec3 OrbitalCamera::CalculatePosition() const {
    float radPitch = glm::radians(Pitch);
    float radYaw = glm::radians(Yaw);

    float x = Distance * cos(radPitch) * cos(radYaw);
    float y = Distance * sin(radPitch);
    float z = Distance * cos(radPitch) * sin(radYaw);

    return Target + glm::vec3(-x, -y, -z);  // camera orbits *around* target
}

glm::mat4 OrbitalCamera::GetViewMatrix() const {
    glm::vec3 position = CalculatePosition();
    return glm::lookAt(position, Target, glm::vec3(0, 1, 0));
}

glm::vec3 OrbitalCamera::GetCameraPosition() const {
    return CalculatePosition();
}

void OrbitalCamera::HandleMouseMotion(int xrel, int yrel) {
    Yaw   += xrel * Sensitivity;
    Pitch -= yrel * Sensitivity;

    if (Pitch > 89.0f)  Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
}

void OrbitalCamera::HandleMouseScroll(float yoffset) {
    Distance -= yoffset;
    if (Distance < MinDistance) Distance = MinDistance;
    if (Distance > MaxDistance) Distance = MaxDistance;
}

void OrbitalCamera::HandleKeyboard(const Uint8* state) {
    glm::vec3 forward = glm::normalize(Target - CalculatePosition());
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

    if (state[SDL_SCANCODE_W]) Target += forward * MoveSpeed * deltaTime;
    if (state[SDL_SCANCODE_S]) Target -= forward * MoveSpeed * deltaTime;
    if (state[SDL_SCANCODE_A]) Target -= right * MoveSpeed * deltaTime;
    if (state[SDL_SCANCODE_D]) Target += right * MoveSpeed * deltaTime;
}


void OrbitalCamera::HandlePan(float xoffset, float yoffset) {
    // Calculate the right and up vectors
    glm::vec3 position = CalculatePosition();
    glm::vec3 front = glm::normalize(Target - position);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    float panSpeed = 0.005f * Distance; // Scale based on distance from target

    Target += -right * xoffset * panSpeed;
    Target += up * yoffset * panSpeed;
}


void OrbitalCamera::HandleEvent(SDL_Event& event, Window& win, OrbitalCamera &camera, bool& running){
    //SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
    
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                break;
    
            case SDL_MOUSEBUTTONDOWN:
                SDL_WarpMouseInWindow(win.window, win.width / 2, win.height / 2);
                break;
    
            case SDL_MOUSEMOTION:
                camera.HandleMouseMotion(event.motion.xrel, event.motion.yrel);
                break;
    
            case SDL_MOUSEWHEEL:
                camera.HandleMouseScroll(static_cast<float>(event.wheel.y));
                break;
            
            
        }
    }
    
    // Update camera delta time
    camera.UpdateDeltaTime();
    
    // Handle keyboard input (WASD, etc.)
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    camera.HandleKeyboard(keyState);
}
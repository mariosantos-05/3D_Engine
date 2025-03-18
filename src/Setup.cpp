#include "Setup.h"

unsigned int cubeTexture;
unsigned int cubeNormalMap;
unsigned int pyramidTexture;
unsigned int pyramidNormalMap;
unsigned int sphereTexture;
unsigned int sphereNormalMap;

Shader myShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
Shader gridShader("shaders/grid_vertex.glsl", "shaders/grid_fragment.glsl");
Grid grid(500.0f, 1.0f, Grid::XZ_PLANE);


Setup::Setup(int width, int height, const char* title)
    : win(width, height, title), camera(), deltaTime(0.0f), lastFrame(0.0f), 
      lastX(width / 2), lastY(height / 2), firstMouse(true){
    if (!init()) std::cerr << "Initialization failed!" << std::endl;

    
}

Setup::~Setup() {
    // Cleanup resources
}

bool Setup::init() {
    if (!win.init()) {
        return false;
    }
    SDL_SetRelativeMouseMode(SDL_TRUE);
    return true;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadTextures();
}

void Setup::use() {
    float currentFrame = SDL_GetTicks() / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return;
            case SDL_MOUSEBUTTONDOWN:
                SDL_WarpMouseInWindow(win.window, win.width / 2, win.height / 2);
                lastX = win.width / 2;
                lastY = win.height / 2;
                break;
            case SDL_MOUSEMOTION:
                handleMouseMotion(event.motion.xrel, event.motion.yrel);
                camera.ProcessMouseMovement(event.motion.xrel, event.motion.yrel);  // Update camera
                break;
            case SDL_MOUSEWHEEL:
                handleMouseWheel(event.wheel.y);  // Zoom in/out
                break;
        }
    }
    processInput();

    // Clear buffers
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Prepare camera
    glm::mat4 view = camera.GetViewMatrix();  // Get the view matrix from the orbital camera
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)win.width / win.height, 0.1f, 100.0f);
    
    myShader.use();
    myShader.setMat4("view", glm::value_ptr(view));
    myShader.setMat4("projection", glm::value_ptr(projection));
    myShader.setVec3("viewPos", camera.GetCameraPosition());

    setupLights();
}

void Setup::processInput() {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Close window on ESC key
    if (state[SDL_SCANCODE_ESCAPE]) {
        SDL_Event quitEvent;
        quitEvent.type = SDL_QUIT;
        SDL_PushEvent(&quitEvent);
    }

    // Camera movement
    float cameraSpeed = 2.5f * deltaTime;
    if (state[SDL_SCANCODE_W])
        camera.GetCameraPosition() += cameraSpeed * camera.GetCameraPosition();
    if (state[SDL_SCANCODE_S])
        camera.GetCameraPosition() -= cameraSpeed * camera.GetCameraPosition();
    if (state[SDL_SCANCODE_A])
        camera.GetCameraPosition() -= glm::normalize(glm::cross(camera.GetCameraPosition(), camera.GetCameraPosition())) * cameraSpeed;
    if (state[SDL_SCANCODE_D])
        camera.GetCameraPosition() += glm::normalize(glm::cross(camera.GetCameraPosition(), camera.GetCameraPosition())) * cameraSpeed;
}

glm::mat4 Setup::getViewMatrix() {
    return camera.GetViewMatrix();
}

glm::mat4 Setup::getProjectionMatrix() {
    return glm::perspective(glm::radians(45.0f), (float)win.width / win.height, 0.1f, 100.0f);
}

void Setup::grid_draw(){
    grid.Draw(gridShader, view, projection);
}

void Setup::handleMouseMotion(int xrel, int yrel) {
    float sensitivity = 0.1f; // Mouse sensitivity
    float xOffset = xrel * sensitivity;
    float yOffset = yrel * sensitivity;

    camera.ProcessMouseMovement(xOffset, yOffset); // Update the orbital camera angles
}

void Setup::handleMouseWheel(int yoffset) {
    camera.ProcessMouseScroll(yoffset);  // Zoom in/out the camera
}


void Setup::loadTextures() {
    cubeTexture = loadTexture("assets/oak_veneer_01_diff_4k.jpg");
    cubeNormalMap = loadTexture("assets/oak_veneer_01_nor_gl_1k.jpg");

    pyramidTexture = loadTexture("assets/stonebase.png");
    pyramidNormalMap = loadTexture("assets/stonenormal.png");

    sphereTexture = loadTexture("assets/Metal_007_basecolor.png");
    sphereNormalMap = loadTexture("assets/Metal_007_normal.png");

    if (!cubeTexture || !pyramidTexture || !sphereTexture) {
        std::cerr << "Texture loading failed!" << std::endl;
        exit(-1);
    }
}


void Setup::setupLights() {
    const int NR_LIGHTS = 2;
    glm::vec3 lightPositions[NR_LIGHTS] = {
        glm::vec3(-6.2f, 3.0f, 2.0f),
        glm::vec3(6.0f, -2.0f, 0.0f)
    };
    glm::vec3 lightColors[NR_LIGHTS] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    };
    for (int i = 0; i < NR_LIGHTS; ++i) {
        myShader.setVec3("lights[" + std::to_string(i) + "].position", lightPositions[i]);
        myShader.setVec3("lights[" + std::to_string(i) + "].color", lightColors[i]);
    }
}



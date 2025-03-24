#include "Setup.h"


OrbitalCamera camera(glm::vec3(0.0f), 10.0f, -90.0f, 0.0f);
Window win(800, 600, "Main");

// Shaders
Shader myShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
Shader gridShader("shaders/grid_vertex.glsl", "shaders/grid_fragment.glsl");

// Objects
Grid grid(500.0f, 1.0f, Grid::XZ_PLANE);



Setup::Setup(int width, int height, const char* title)
    : win(width, height, title), camera(){
    if (!win.init()) std::cerr << "Initialization failed!" << std::endl;

    
}

void Setup::Init(){
    // Main loop
    bool running = true;
    while (running) {

        SDL_Event event;
        camera.HandleEvent(event,win, camera, running);
        

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


        // Draw Grid
        grid.Draw(gridShader, view, projection);

        SDL_GL_SwapWindow(win.window);
    }
}


Setup::~Setup() {
    // Cleanup resources
}




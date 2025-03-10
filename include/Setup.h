
struct Window {
    SDL_Window* window;
    SDL_GLContext glContext;
    int width;
    int height;
    const char* title;

    // Constructor to initialize the window parameters
    Window(int w, int h, const char* t) 
        : width(w), height(h), title(t), window(nullptr), glContext(nullptr) {}

    // Initialize the window
    bool init() {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Failed to initialize SDL! Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Set OpenGL attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // Create a window
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
        if (!window) {
            std::cerr << "Failed to create SDL window! Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        // Create an OpenGL context
        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            std::cerr << "Failed to create OpenGL context! Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }

        // Load OpenGL functions using Glad
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
            SDL_GL_DeleteContext(glContext);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

        // Enable relative mouse mode for smooth camera control
        SDL_SetRelativeMouseMode(SDL_TRUE);

        return true;
    }

    // Destructor to clean up SDL
    ~Window() {
        if (glContext) {
            SDL_GL_DeleteContext(glContext);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }
};

// Global variables for camera movement and orientation
glm::vec3 cameraPos = glm::vec3(-7.0f, 1.0f, 0.0f);  // Camera Position
glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f); // Camera Direction
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);    // Up vector for the camera

float yaw = -90.0f;   // Yaw angle (left-right)
float pitch = 0.0f;   // Pitch angle (up-down)
float lastX = 400, lastY = 300;  // Mouse position tracking
bool firstMouse = true; // Flag to initialize the mouse position

float deltaTime = 0.0f;  // Time between current and last frame
float lastFrame = 0.0f;  // Time of last frame

void processInput(SDL_Window* window) {
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
        cameraPos += cameraSpeed * cameraFront;
    if (state[SDL_SCANCODE_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (state[SDL_SCANCODE_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (state[SDL_SCANCODE_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void handleMouseMotion(int xrel, int yrel) {
    float sensitivity = 0.1f; // Mouse sensitivity
    float xOffset = xrel * sensitivity;
    float yOffset = yrel * sensitivity;

    yaw += xOffset;
    pitch -= yOffset; // Inverted y-axis

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

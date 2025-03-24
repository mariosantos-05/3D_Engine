#include <glad/glad.h>
#include "Setup.h"
#include "Globals.h"

int main() {
    Setup set(1000, 800, "teste");
    set.Shaders();
    set.Init();
    

    return 0;
}

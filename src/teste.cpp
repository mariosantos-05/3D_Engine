#include <glad/glad.h>
#include "Setup.h"
#include "Globals.h"

int main() {
    Setup set(800, 600, "teste");
    set.Init();

    return 0;
}

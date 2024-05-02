#include "Core.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HrothCore {
    int testVariable = 42;

    bool initGLFW()
    {
        return glfwInit();
    }

    bool init()
    {
        return gladLoadGL();
    }
}
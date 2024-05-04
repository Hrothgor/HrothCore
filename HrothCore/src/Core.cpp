#include "Core.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HrothCore {
    int testVariable = 42;
    glm::vec3 testVec = glm::vec3(1.0f, 2.0f, 3.0f);

    bool initGLFW()
    {
        return glfwInit();
    }

    bool init()
    {
        return gladLoadGL();
    }
}
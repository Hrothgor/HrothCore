#include "Core.hpp"
#include <glad/glad.h>

namespace HrothCore {
    int testVariable = 42;

    bool init()
    {
        return gladLoadGL();
    }
}
#pragma once

#include "HrothCore/Core/KeyCodes.hpp"

namespace HrothCore
{
    namespace Input 
    {
        bool IsKeyPressed(KeyCode keyCode);
        bool IsKeyReleased(KeyCode keyCode);

        bool IsMouseButtonPressed(MouseButton mouseCode);
        bool IsMouseButtonReleased(MouseButton mouseCode);
        glm::vec2 GetMousePosition();
    };
}
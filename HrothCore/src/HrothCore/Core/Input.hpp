#pragma once

#include "HrothCore/Core/KeyCodes.hpp"

namespace HrothCore
{
    class Input 
    {

        static bool IsKeyPressed(KeyCode keyCode);
        static bool IsKeyReleased(KeyCode keyCode);

        static bool IsMouseButtonPressed(MouseButton mouseCode);
        static bool IsMouseButtonReleased(MouseButton mouseCode);
        static glm::vec2 GetMousePosition();
    };
}
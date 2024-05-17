#pragma once

#include "HrothCore/Events/EventManager.hpp"
#include "HrothCore/Core/KeyCodes.hpp"

namespace HrothCore
{
    struct KeyPressedEvent : public Event
    {
        HC_STRINGIFY_CLASS(KeyPressedEvent);
        
        KeyCode Code;
        ModCode Modifier;

        KeyPressedEvent(KeyCode code, ModCode modifier)
            : Code(code), Modifier(modifier) {}
    };

    struct KeyReleasedEvent : public Event
    {
        HC_STRINGIFY_CLASS(KeyReleasedEvent);
        
        KeyCode Code;
        ModCode Modifier;

        KeyReleasedEvent(KeyCode code, ModCode modifier)
            : Code(code), Modifier(modifier) {}
    };

    struct KeyRepeatedEvent : public Event
    {
        HC_STRINGIFY_CLASS(KeyRepeatedEvent);
        
        KeyCode Code;
        ModCode Modifier;

        KeyRepeatedEvent(KeyCode code, ModCode modifier)
            : Code(code), Modifier(modifier) {}
    };

    struct MousePressedEvent : public Event
    {
        HC_STRINGIFY_CLASS(MousePressedEvent);
    
        MouseButton Button;
        ModCode Modifier;

        MousePressedEvent(MouseButton button, ModCode modifier)
            : Button(button), Modifier(modifier) {}
    };

    struct MouseReleasedEvent : public Event
    {
        HC_STRINGIFY_CLASS(MouseReleasedEvent);
    
        MouseButton Button;
        ModCode Modifier;

        MouseReleasedEvent(MouseButton button, ModCode modifier)
            : Button(button), Modifier(modifier) {}
    };

    struct MouseScrolledEvent : public Event
    {
        HC_STRINGIFY_CLASS(MouseScrolledEvent);

        double XOffset;
        double YOffset;

        MouseScrolledEvent(double xOffset, double yOffset)
            : XOffset(xOffset), YOffset(yOffset) {}
    };
}
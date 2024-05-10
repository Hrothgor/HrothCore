#pragma once

#include "HrothCore/Events/EventManager.hpp"

namespace HrothCore
{
    struct KeyPressedEvent : public Event
    {
        HC_STRINGIFY_CLASS(KeyPressedEvent);
        
        uint32_t KeyCode;
        uint32_t Modifier;

        KeyPressedEvent(uint32_t keyCode, uint32_t modifier)
            : KeyCode(keyCode), Modifier(modifier) {}
    };

    struct KeyReleasedEvent : public Event
    {
        HC_STRINGIFY_CLASS(KeyReleasedEvent);
        
        uint32_t KeyCode;
        uint32_t Modifier;

        KeyReleasedEvent(uint32_t keyCode, uint32_t modifier)
            : KeyCode(keyCode), Modifier(modifier) {}
    };

    struct KeyRepeatedEvent : public Event
    {
        HC_STRINGIFY_CLASS(KeyRepeatedEvent);
        
        uint32_t KeyCode;
        uint32_t Modifier;

        KeyRepeatedEvent(uint32_t keyCode, uint32_t modifier)
            : KeyCode(keyCode), Modifier(modifier) {}
    };

    struct MousePressedEvent : public Event
    {
        HC_STRINGIFY_CLASS(MousePressedEvent);
    
        uint32_t Button;
        uint32_t Modifier;

        MousePressedEvent(uint32_t button, uint32_t modifier)
            : Button(button), Modifier(modifier) {}
    };

    struct MouseReleasedEvent : public Event
    {
        HC_STRINGIFY_CLASS(MouseReleasedEvent);
    
        uint32_t Button;
        uint32_t Modifier;

        MouseReleasedEvent(uint32_t button, uint32_t modifier)
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
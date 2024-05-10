#pragma once

#include "HrothCore/Events/EventManager.hpp"

namespace HrothCore
{
    struct WindowResizeEvent : public Event
    {
        HC_STRINGIFY_CLASS(WindowResizeEvent);
        
        uint32_t Width;
        uint32_t Height;

        WindowResizeEvent(uint32_t width, uint32_t height)
            : Width(width), Height(height) {}
    };

    struct WindowCloseEvent : public Event
    {
        HC_STRINGIFY_CLASS(WindowCloseEvent);
    };

    struct WindowFocusEvent : public Event
    {
        HC_STRINGIFY_CLASS(WindowFocusEvent);
    };

    struct WindowLostFocusEvent : public Event
    {
        HC_STRINGIFY_CLASS(WindowLostFocusEvent);
    };
}
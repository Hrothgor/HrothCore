#pragma once

#include <glm/glm.hpp>

namespace HrothCore
{
    enum Key : uint16_t
    {
        // From glfw3.h
        Space               = 32,
        Apostrophe          = 39, /* ' */
        Comma               = 44, /* , */
        Minus               = 45, /* - */
        Period              = 46, /* . */
        Slash               = 47, /* / */

        NUM_0               = 48, /* 0 */
        NUM_1               = 49, /* 1 */
        NUM_2               = 50, /* 2 */
        NUM_3               = 51, /* 3 */
        NUM_4               = 52, /* 4 */
        NUM_5               = 53, /* 5 */
        NUM_6               = 54, /* 6 */
        NUM_7               = 55, /* 7 */
        NUM_8               = 56, /* 8 */
        NUM_9               = 57, /* 9 */

        Semicolon           = 59, /* ; */
        Equal               = 61, /* = */

        A                   = 65,
        B                   = 66,
        C                   = 67,
        D                   = 68,
        E                   = 69,
        F                   = 70,
        G                   = 71,
        H                   = 72,
        I                   = 73,
        J                   = 74,
        K                   = 75,
        L                   = 76,
        M                   = 77,
        N                   = 78,
        O                   = 79,
        P                   = 80,
        Q                   = 81,
        R                   = 82,
        S                   = 83,
        T                   = 84,
        U                   = 85,
        V                   = 86,
        W                   = 87,
        X                   = 88,
        Y                   = 89,
        Z                   = 90,

        LeftBracket         = 91,  /* [ */
        Backslash           = 92,  /* \ */
        RightBracket        = 93,  /* ] */
        GraveAccent         = 96,  /* ` */

        World1              = 161, /* non-US #1 */
        World2              = 162, /* non-US #2 */

        /* Function keys */
        Escape              = 256,
        Enter               = 257,
        Tab                 = 258,
        Backspace           = 259,
        Insert              = 260,
        Delete              = 261,
        Right               = 262,
        Left                = 263,
        Down                = 264,
        Up                  = 265,
        PageUp              = 266,
        PageDown            = 267,
        Home                = 268,
        End                 = 269,
        CapsLock            = 280,
        ScrollLock          = 281,
        NumLock             = 282,
        PrintScreen         = 283,
        Pause               = 284,
        F1                  = 290,
        F2                  = 291,
        F3                  = 292,
        F4                  = 293,
        F5                  = 294,
        F6                  = 295,
        F7                  = 296,
        F8                  = 297,
        F9                  = 298,
        F10                 = 299,
        F11                 = 300,
        F12                 = 301,
        F13                 = 302,
        F14                 = 303,
        F15                 = 304,
        F16                 = 305,
        F17                 = 306,
        F18                 = 307,
        F19                 = 308,
        F20                 = 309,
        F21                 = 310,
        F22                 = 311,
        F23                 = 312,
        F24                 = 313,
        F25                 = 314,

        /* Keypad */
        KEYPAD_0            = 320,
        KEYPAD_1            = 321,
        KEYPAD_2            = 322,
        KEYPAD_3            = 323,
        KEYPAD_4            = 324,
        KEYPAD_5            = 325,
        KEYPAD_6            = 326,
        KEYPAD_7            = 327,
        KEYPAD_8            = 328,
        KEYPAD_9            = 329,
        KEYPAD_Decimal      = 330,
        KEYPAD_Divide       = 331,
        KEYPAD_Multiply     = 332,
        KEYPAD_Subtract     = 333,
        KEYPAD_Add          = 334,
        KEYPAD_Enter        = 335,
        KEYPAD_Equal        = 336,

        LeftShift           = 340,
        LeftControl         = 341,
        LeftAlt             = 342,
        LeftSuper           = 343,
        RightShift          = 344,
        RightControl        = 345,
        RightAlt            = 346,
        RightSuper          = 347,
        Menu                = 348
    };

    enum Mouse : uint16_t
    {
            // From glfw3.h
			Button_0               = 0,
			Button_1               = 1,
			Button_2               = 2,
			Button_3               = 3,
			Button_4               = 4,
			Button_5               = 5,
			Button_6               = 6,
			Button_7               = 7,

			ButtonLast             = Button_7,
			ButtonLeft             = Button_0,
			ButtonRight            = Button_1,
			ButtonMiddle           = Button_2
    };

    class Input 
    {

        static bool IsKeyPressed(Key keyCode);
        static bool IsKeyReleased(Key keyCode);

        static bool IsMouseButtonPressed(Mouse mouseCode);
        static bool IsMouseButtonReleased(Mouse mouseCode);
        static glm::vec2 GetMousePosition();
    };
}
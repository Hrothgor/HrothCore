#pragma once

#if DEBUG
    #include <csignal>

    #if _WIN32
        #define HC_DEBUGBREAK() __debugbreak()
    #else
        #define HC_DEBUGBREAK() raise(SIGTRAP)
    #endif

    #define HC_ASSERT(check) if (!(check)) { std::cerr << "Assertion '" << #check << "' failed at " << std::filesystem::path(__FILE__).filename().string() << ":" << __LINE__; HC_DEBUGBREAK(); }
#else
    #define HC_DEBUGBREAK()
    #define HC_ASSERT(check)
#endif
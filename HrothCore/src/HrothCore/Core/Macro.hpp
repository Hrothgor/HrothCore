#pragma once

#if DEBUG
    #include "HrothCore/Core/Logger.hpp"
    #include <csignal>

    #if _WIN32
        #define HC_DEBUGBREAK() __debugbreak()
    #else
        #define HC_DEBUGBREAK() raise(SIGTRAP)
    #endif

    #define HC_ASSERT(check) if (!(check)) { HC_LOG_CRITICAL("Assertion '{0}' failed at {1}:{2}", #check, std::filesystem::path(__FILE__).filename().string(), __LINE__); HC_DEBUGBREAK(); }
#else
    #define HC_DEBUGBREAK() void(0)
    #define HC_ASSERT(check) void(0)
#endif

#define HC_SINGLETON(className)                                                  \
    private:                                                                     \
        className() = default;                                                   \
    public:                                                                      \
        className(const className&) = delete;                                    \
        className& operator=(const className &) = delete;                        \
        className(className &&) = delete;                                        \
        className & operator=(className &&) = delete;                            \
        static className& Get() {                                                \
            static const std::unique_ptr<className> s_Instance(new className()); \
            return *s_Instance;                                                  \
        }

#define HC_STRINGIFY_CLASS(class) virtual const char *ToString() const { return #class; }

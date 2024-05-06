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

#define HC_SINGLETON(className)                                                  \
    private:                                                                     \
        className();                                                             \
    public:                                                                      \
        className(const className&) = delete;                                    \
        className& operator=(const className &) = delete;                        \
        className(className &&) = delete;                                        \
        className & operator=(className &&) = delete;                            \
        static className& Get() {                                                \
            static const std::unique_ptr<className> s_Instance(new className()); \
            return *s_Instance;                                                  \
        }

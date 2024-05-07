#pragma once

#include "HrothCore/Core/Macro.hpp"

#define HC_REGISTER_EVENT(eventType, fn) HrothCore::EventManager::Get().RegisterToEvent<eventType>(fn)
#define HC_UNREGISTER_EVENT(eventType, handle) HrothCore::EventManager::Get().UnregisterToEvent<eventType>(handle)
#define HC_DISPATCH_EVENT(event) HrothCore::EventManager::Get().DispatchEvent(event)

namespace HrothCore
{
    struct Event
    {
        HC_STRINGIFY_CLASS(Event);
    };

    using EventHandle = uint32_t;

    class EventManager
    {
        HC_SINGLETON(EventManager);

        struct EventRegistration
        {
            EventHandle handle;
            std::function<bool(const Event&)> fn;
        };

        public:
            virtual ~EventManager() = default;

            template<typename T>
            EventHandle RegisterToEvent(std::function<bool(const T&)> fnT)
            {
                static_assert(std::is_base_of_v<Event, T>, "T must be derived from Event");
                
                static EventHandle handle = 0;
                HC_ASSERT(handle < UINT_MAX);
                std::function<bool(const Event&)> fn = [fnT](const Event &event) -> bool
                {
                    return fnT(static_cast<const T&>(event));
                };
                m_Events[typeid(T)].push_back({++handle, fn});
                return handle;
            }

            template<typename T>
            void UnregisterToEvent(EventHandle handle)
            {
                auto it = std::find_if(m_Events[typeid(T)].begin(), m_Events[typeid(T)].end(), 
                    [handle](const EventRegistration &reg) { return reg.handle == handle; });

                if (it == m_Events[typeid(T)].end())
                {
                    return;
                }

                m_Events[typeid(T)].erase(it);
            }

            template<typename T>
            void DispatchEvent(const T &event)
            {
                static_assert(std::is_base_of_v<Event, T>, "T must be derived from Event");
             
                for (auto &fns : m_Events[typeid(T)])
                {
                    if (fns.fn(event))
                    {
                        break;
                    }
                }
            }

        private:
            std::unordered_map<std::type_index,
                            std::vector<EventRegistration>> m_Events;
    };
}
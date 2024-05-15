#pragma once

namespace HrothCore
{
    class IClient;

    class Engine
    {
        HC_SINGLETON(Engine)

        public:
            virtual ~Engine() = default;

            void Init(std::shared_ptr<IClient>& client);
            void Shutdown();
            void Update(double dt);
        private:
            std::shared_ptr<IClient> m_Client;
    };
}
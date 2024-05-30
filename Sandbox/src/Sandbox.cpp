#include <HrothCore.hpp>

#include "Panels/SceneHierarchyPanel.hpp"

namespace HrothCore
{
    class SandboxClient : public IClient
    {
        public:
            SandboxClient() = default;
            virtual ~SandboxClient() = default;

            void Init() override
            {
                HC_LOG_INFO("SandboxClient::Init");
                ImGuiLayer::Get().RegisterPanel<SceneHierarchyPanel>(Engine::Get().GetScene());
            }

            void Shutdown() override
            {
                HC_LOG_INFO("SandboxClient::Shutdown");
            }

            void Update(float dt) override
            {
            }
    };

    HrothCore::Application* HrothCore::CreateApplication(ApplicationCommandLineArgs args)
    {
        HrothCore::ApplicationSpecification specification;
        specification.Name = "Sandbox";
        specification.WorkingDirectory = ASSETS_DIR;
        specification.LogFile = "./Sandbox.log";
        specification.CommandLineArgs = args;

        std::shared_ptr<HrothCore::IClient> client = std::make_shared<SandboxClient>();

        return new HrothCore::Application(specification, client);
    }
}


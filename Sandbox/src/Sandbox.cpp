#include <HrothCore.hpp>

#include "Panels/InfoPanel.hpp"
#include "Panels/SceneHierarchyPanel.hpp"
#include "Panels/ViewportPanel.hpp"

namespace HrothCore
{
    class SandboxClient : public IClient
    {
        public:
            SandboxClient()
                : m_EditorCamera(&m_EditorCameraPositioner)
            {
            }
            virtual ~SandboxClient() = default;

            void Init() override
            {
                HC_LOG_INFO("SandboxClient::Init");
                ImGuiLayer::Get().UseDocking(true);
                ImGuiLayer::Get().RegisterPanel<InfoPanel>();
                ImGuiLayer::Get().RegisterPanel<SceneHierarchyPanel>(Engine::Get().GetScene());
                ImGuiLayer::Get().RegisterPanel<ViewportPanel>(&m_EditorCameraPositioner);

                Engine::Get().SetCameraPtr(&m_EditorCamera);
            }

            void Shutdown() override
            {
                HC_LOG_INFO("SandboxClient::Shutdown");
            }

            void Update(float dt) override
            {
            }
        
        private:
            Camera m_EditorCamera;
            CameraPositionerEditor m_EditorCameraPositioner;
    };

    Application *CreateApplication(ApplicationCommandLineArgs args)
    {
        ApplicationSpecification specification;
        specification.Name = "Sandbox";
        specification.WorkingDirectory = ASSETS_DIR;
        specification.LogFile = "./Sandbox.log";
        specification.CommandLineArgs = args;

        std::shared_ptr<IClient> client = std::make_shared<SandboxClient>();

        return new Application(specification, client);
    }
}


#include <HrothCore.hpp>

class SandboxClient : public HrothCore::IClient
{
    public:
        SandboxClient() = default;
        virtual ~SandboxClient() = default;

        void Init() override
        {
            HC_LOG_INFO("SandboxClient::Init");
        }

        void Shutdown() override
        {
            HC_LOG_INFO("SandboxClient::Shutdown");
        }

        void Update(double dt) override
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
#include <HrothCore.hpp>

class SandboxApplication : public HrothCore::Application
{
    public:
        SandboxApplication(const HrothCore::ApplicationSpecification& specification)
            : HrothCore::Application(specification)
        {
        }

        virtual ~SandboxApplication()
        {
        }
};

HrothCore::Application* HrothCore::CreateApplication(ApplicationCommandLineArgs args)
{
    HrothCore::ApplicationSpecification specification;
    specification.Name = "Sandbox";
    specification.WorkingDirectory = "./";
    specification.CommandLineArgs = args;

    return new SandboxApplication(specification);
}
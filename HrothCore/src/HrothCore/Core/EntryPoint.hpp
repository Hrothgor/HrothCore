#pragma once

#include "HrothCore/Core/Application.hpp"

int main(int argc, char **argv)
{
    HrothCore::Application* app = HrothCore::CreateApplication({argc, argv});
    app->Run();
    delete app;
}
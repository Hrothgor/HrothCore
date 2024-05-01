#include <Core.hpp>
#include <iostream>
#include <glad/glad.h>

int main()
{
    std::cout << HrothCore::init() << std::endl;
    std::cout << gladLoadGL() << std::endl;
    std::cout << HrothCore::testVariable << std::endl;
    return 0;
}
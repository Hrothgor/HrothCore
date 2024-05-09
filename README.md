# HrothCore

## Requirements

 - [Visual Studio Code](https://code.visualstudio.com/)
	 - [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) - C/C++ IntelliSense, debugging, and code browsing. (Must Have)
	 - [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake) - Syntax Highlighting  (Optional)
	 - [CMake Tools ](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) - Extended CMake support in Visual Studio Code (Must Have)
 - [GNU](https://gcc.gnu.org/) / [MinGW](https://www.mingw-w64.org/) / [CLang](https://clang.llvm.org/) or [Visual Studio 2017 Build Tools(or Greater)](https://visualstudio.microsoft.com/downloads/?q=build%20tools) - Compiler
 - [GPU with support for OpenGL® 4.6](https://support.pix4d.com/hc/en-us/articles/203876689-How-to-verify-the-supported-OpenGL-versions-of-the-graphics-card)
 - [CMake](https://cmake.org/) - Minimum Version 3.10

## Build

 - Just run the scripts in the `scripts` folder.
    - You might need to give the scripts permission to execute. (Linux or MacOS) (chmod +x)
    - The scripts will create a `build` folder, build the engine, link it to the project Sandbox and make an executable.
    - You can find the Sandbox executable in the `bin` folder (if not find it in `build`).

@echo off 

set GLFW_ROOT=C:/lib/glfw-3.3.4.bin.WIN64
set GLEW_ROOT=C:/lib/glew-2.1.0_x64
set stb_image_ROOT=C:/lib/stb
set ComputeEngine_ROOT=C:/VisualStudioProjects/ComputeEngine

cmake -B build -S . -DGLFW_ROOT=%GLFW_ROOT% -DGLEW_ROOT=%GLEW_ROOT% -Dstb_image_ROOT=%stb_image_ROOT% -DComputeEngine_ROOT=%ComputeEngine_ROOT%
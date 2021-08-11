@echo off 

set GLFW_ROOT=
set GLEW_ROOT=
set stb_image_ROOT=
set ComputeEngine_ROOT=%CD%/ComputeEngine

cmake -B ComputeEngine/build -S ComputeEngine -DGLFW_ROOT=%GLFW_ROOT% -DGLEW_ROOT=%GLEW_ROOT% -Dstb_image_ROOT=%stb_image_ROOT%
cmake --build ComputeEngine/build --config Release
cmake -B build -S . -DGLFW_ROOT=%GLFW_ROOT% -DGLEW_ROOT=%GLEW_ROOT% -Dstb_image_ROOT=%stb_image_ROOT% -DComputeEngine_ROOT=%ComputeEngine_ROOT%
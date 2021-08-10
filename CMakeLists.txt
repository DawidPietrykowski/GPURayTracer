cmake_minimum_required(VERSION 3.10)

# So library linking is more sane.
cmake_policy(SET CMP0003 NEW)

# So syntax problems are errors.
cmake_policy(SET CMP0010 NEW)

# Input directories must have CMakeLists.txt
cmake_policy(SET CMP0014 NEW)

# Compile definitions.
cmake_policy(SET CMP0043 NEW)

# Use ROOT variables in find_package.
cmake_policy(SET CMP0074 NEW)

# Convert relative paths to absolute in target_sources()
cmake_policy(SET CMP0076 NEW)

# Copy files from source directory to destination directory, substituting any
# variables.  Create destination directory if it does not exist.

macro(configure_files srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    @ONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)

# Initialize project

project(GPURayTracer)

# Add executable
add_executable(${PROJECT_NAME})

set(default_build_type "Release")
set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Find GLFW

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h 
HINTS 
${CMAKE_CURRENT_SOURCE_DIR}/include 
${CMAKE_CURRENT_SOURCE_DIR}/lib 
${GLFW_ROOT} 
${GLFW_ROOT}/include)

if(GLFW_INCLUDE_DIR)
message(STATUS "GLFW_INCLUDE_DIR: ${GLFW_INCLUDE_DIR}")
else()
message(FATAL_ERROR "glfw3.h not found")
endif()

find_path(GLFW_LIB_DIR glfw3.lib 
HINTS 
${CMAKE_CURRENT_SOURCE_DIR}/include 
${CMAKE_CURRENT_SOURCE_DIR}/lib 
${GLFW_ROOT}/lib 
${GLFW_ROOT}/lib-vc2019 
${GLFW_ROOT}/lib-vc2017 
${GLFW_ROOT}/lib-vc2015)

if(GLFW_LIB_DIR)
message(STATUS "GLFW_LIB_DIR: ${GLFW_LIB_DIR}")
else()
message(FATAL_ERROR "glfw3.lib not found")
endif()

# Find GLEW

find_package(GLEW REQUIRED)

if(GLEW_FOUND)
set(GLEW_INCLUDE_DIR ${GLEW_INCLUDE_DIRS})
get_filename_component(GLEW_LIBRARIES ${GLEW_LIBRARIES} DIRECTORY)
message(STATUS "GLEW_INCLUDE_DIR: ${GLEW_INCLUDE_DIR}")
message(STATUS "GLEW_LIBRARIES: ${GLEW_LIBRARIES}")
else()
message(FATAL_ERROR "GLEW not found")
endif()

# Find OpenGL

if(NOT WIN32)
    find_package(OpenGL REQUIRED)
endif()

# Find stb_image

find_path(stb_image_INCLUDE_DIR stb_image.h 
HINTS
${CMAKE_CURRENT_SOURCE_DIR}/include 
${CMAKE_CURRENT_SOURCE_DIR}/include/stb
${CMAKE_CURRENT_SOURCE_DIR}/lib  
${stb_image_ROOT})

if(stb_image_INCLUDE_DIR)
message(STATUS "stb_image_INCLUDE_DIR: ${stb_image_INCLUDE_DIR}")
else()
message(FATAL_ERROR "stb_image not found")
endif()

# Find ComputeEngine

find_path(ComputeEngine_LIB_DIR ComputeEngine.lib 
HINTS
${CMAKE_CURRENT_SOURCE_DIR}/include 
${CMAKE_CURRENT_SOURCE_DIR}/lib 
${ComputeEngine_ROOT} 
${ComputeEngine_ROOT}/lib 
${ComputeEngine_ROOT}/lib/Release
${ComputeEngine_ROOT}/build/lib 
${ComputeEngine_ROOT}/build/lib/Release)

if(ComputeEngine_LIB_DIR)
message(STATUS "ComputeEngine_LIB_DIR: ${ComputeEngine_LIB_DIR}")
else()
message(FATAL_ERROR "ComputeEngine.lib not found")
endif()

find_path(ComputeEngine_INCLUDE_DIR ComputeEngine.h 
HINTS 
${CMAKE_CURRENT_SOURCE_DIR}/include 
${CMAKE_CURRENT_SOURCE_DIR}/lib 
${ComputeEngine_ROOT} 
${ComputeEngine_ROOT}/include 
${ComputeEngine_ROOT}/build/include)

if(ComputeEngine_INCLUDE_DIR)
message(STATUS "ComputeEngine_INCLUDE_DIR: ${ComputeEngine_INCLUDE_DIR}")
else()
message(FATAL_ERROR "ComputeEngine.h not found")
endif()

# Add source

add_subdirectory(src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
${GLFW_INCLUDE_DIR}
${OPENGL_INCLUDE_DIR}
${stb_image_INCLUDE_DIR}
${ComputeEngine_INCLUDE_DIR}
${GLEW_INCLUDE_DIR})

target_link_directories(${PROJECT_NAME} PUBLIC ${GLFW_LIB_DIR} ${GLEW_LIBRARIES} ${ComputeEngine_LIB_DIR})
target_link_libraries(${PROJECT_NAME} "glfw3.lib" "opengl32.lib" "glew32s.lib" "ComputeEngine.lib")

# Copy shaders, assets and configs for binaries to access
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR}/Release)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/Release)
file(COPY configs DESTINATION ${CMAKE_BINARY_DIR}/Release)
cmake_minimum_required(VERSION 3.27)
project(CelShader)

add_executable(CelShader main.cpp)

add_library(utils STATIC camera.cpp mesh_manager.cpp shader_manager.cpp)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

target_include_directories(CelShader PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(CelShader PUBLIC ${OPEN_GL_INCLUDE_DIRS})

target_link_libraries(CelShader PUBLIC utils glfw GLEW::glew OpenGL::GL assimp)
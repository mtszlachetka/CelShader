cmake_minimum_required(VERSION 3.27)
project(CelShader)

add_executable(CelShader src/main.cpp)

add_subdirectory(src)
add_subdirectory(lib)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

target_include_directories(CelShader PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(CelShader PUBLIC ${OPEN_GL_INCLUDE_DIRS})

target_link_libraries(CelShader PUBLIC utils SOIL glfw GLEW::glew OpenGL::GL assimp)
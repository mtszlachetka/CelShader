add_library(utils STATIC camera.cpp mesh_manager.cpp shader_manager.cpp texture_manager.cpp)
target_include_directories(utils PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(utils PUBLIC "${PROJECT_SOURCE_DIR}/lib/SOIL")
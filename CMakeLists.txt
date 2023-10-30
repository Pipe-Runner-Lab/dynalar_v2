cmake_minimum_required(VERSION 3.0.0)

# Set the project name
set(TARGET dynalar_v2)

project(${TARGET} VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# Add source files
file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

# Add executable target
add_executable(${TARGET} ${SOURCES})

target_compile_features(${TARGET} PRIVATE cxx_std_17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET} PRIVATE OpenGL::GL)

# GLEW
find_package(GLEW REQUIRED)
target_link_libraries(${TARGET} PRIVATE GLEW::GLEW)

# GLFW
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${TARGET} PRIVATE glfw)

# GLM
find_package(glm CONFIG REQUIRED)
target_link_libraries(${TARGET} PRIVATE glm::glm)

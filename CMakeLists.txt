cmake_minimum_required(VERSION 3.0.0)

# Set the project name
set(TARGET dynalar_v2)

project(${TARGET} VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# Add source files
file(GLOB_RECURSE LIB_SOURCES src/*.cpp src/*.h)

# Add library target
# add_library(${TARGET} ${LIB_SOURCES})
add_executable(${TARGET} ${LIB_SOURCES})

target_compile_features(${TARGET} PRIVATE cxx_std_17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# --------------------------- PREPROCESSOR FLAGS --------------------------- #
add_compile_definitions($<$<CONFIG:Debug>:_DEBUG>)


# --------------------------- INCLUDE DIRECTORIES --------------------------- #
# https://stackoverflow.com/questions/15662497/difference-between-cmake-current-source-dir-and-cmake-current-list-dir
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/third-party)

# -------------------------------------------------------------------------- #
#                                    VCPKG                                   #
# -------------------------------------------------------------------------- #

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

# imgui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${TARGET} PRIVATE imgui::imgui)

# imguizmo
find_package(imguizmo CONFIG REQUIRED)
target_link_libraries(${TARGET} PRIVATE imguizmo::imguizmo)

# Assimp
find_package(assimp CONFIG REQUIRED)
target_link_libraries(${TARGET} PRIVATE assimp::assimp)

# fmt
find_package(fmt CONFIG REQUIRED)
target_link_libraries(${TARGET} PRIVATE fmt::fmt)

# stb
find_package(Stb REQUIRED)
target_include_directories(${TARGET} PRIVATE ${Stb_INCLUDE_DIR})

# # -------------------------------------------------------------------------- #
# #                            SANITIZER SETTINGS                              #
# # -------------------------------------------------------------------------- #
# TODO: Explore more into these options
# target_link_options(${TARGET} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
# target_compile_options(${TARGET} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)

# # -------------------------------------------------------------------------- #
# #                                 EXECUTABLE                                 #
# # -------------------------------------------------------------------------- #

# file(GLOB_RECURSE APP_SOURCES src/*.cpp src/*.h)

# # Add executable target
# add_executable(${TARGET}_app ${APP_SOURCES})

# target_compile_features(${TARGET}_app PRIVATE cxx_std_17)

# # Link libraries
# target_link_libraries(${TARGET}_app PRIVATE ${TARGET})
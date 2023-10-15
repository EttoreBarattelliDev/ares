cmake_minimum_required(VERSION 3.1...3.26)

# Project definition
project(
  ares
  VERSION 0.0.1
  LANGUAGES CXX)

# C++ standard version
set (CMAKE_CXX_STANDARD 11)
#set (CMAKE_CXX_FLAGS -no-pie)

# Required packages
find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)
find_package(X11 REQUIRED)
find_package(PNG REQUIRED)

# Library definitions
add_library(ares SHARED)
add_library(gltf SHARED)
add_library(port SHARED)

# Include dirs
include_directories(include)

# Subdirectories for libs
add_subdirectory(src)

target_include_directories(gltf PRIVATE third-party/tinygltf)

# Link libraries for libs
target_link_libraries(port PRIVATE X11)
target_link_libraries(gltf PRIVATE ares)
target_link_libraries(ares PRIVATE EGL GLESv2 png port)

# Test application
add_executable(gltf_test)
add_executable(normal_map_test)
add_subdirectory(tests)
target_link_libraries(gltf_test PRIVATE ares gltf port)
target_link_libraries(normal_map_test PRIVATE ares port)

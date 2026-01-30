# chatgpt was used to make this file

cmake_minimum_required(VERSION 3.10)
project(my_opengl_project VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# --- 1. Configure Dependencies (Submodules) ---

# GLFW Settings: disable things we don't need to speed up building
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/glfw)

# Freetype Settings: disable its installation and demos
# This ensures CMake knows how to build the 'freetype' target
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE) 
add_subdirectory(dependencies/freetype-2.14.1)

# --- 2. Define the Executable ---

add_executable(my_opengl_project
    src/main.cpp
    dependencies/glad.c
)

# --- 3. Header Includes (Relative Paths) ---

target_include_directories(my_opengl_project PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype-2.14.1/include
)

# --- 4. Linking Libraries ---

find_package(OpenGL REQUIRED)

target_link_libraries(my_opengl_project
    PRIVATE
    ${OPENGL_LIBRARIES}
    glfw        # Links to the target created by add_subdirectory(dependencies/glfw)
    freetype    # Links to the target created by add_subdirectory(dependencies/freetype-2.14.1)
)

# Windows-specific: Ensure we link the standard Windows GDI/User libraries 
# that GLFW and OpenGL need to create a window.
if(WIN32)
    target_link_libraries(my_opengl_project PRIVATE gdi32)
endif()
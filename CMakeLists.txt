cmake_minimum_required(VERSION 3.10)
project(my_opengl_project VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# --- Add executable first ---
add_executable(my_opengl_project
    src/main.cpp
    dependencies/glad.c
)

# --- Freetype setup ---
include_directories("/home/dev/projects_for_fun/opengl_projects/worldgen/dependencies/freetype-2.14.1/include")
link_directories("/home/dev/projects_for_fun/opengl_projects/worldgen/dependencies/freetype-2.14.1/objs/.libs")

# Link Freetype (no 'lib' prefix!)
target_link_libraries(my_opengl_project PRIVATE freetype)

# --- Find and include OpenGL ---
find_package(OpenGL REQUIRED)

# --- Link OpenGL + GLFW + GLEW ---
target_link_libraries(my_opengl_project
    PRIVATE
    ${OPENGL_LIBRARIES}  # Core OpenGL
    glfw                 # Window/context/input
    GLEW                 # Extension wrangler (only if you actually use it)
)

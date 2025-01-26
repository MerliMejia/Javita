cmake_minimum_required(VERSION 3.10)
project(JavitasProject)

set(CMAKE_CXX_STANDARD 17)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add GLAD
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Collect all .cpp files in the src directory
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# Add the executable
add_executable(Javitas ${SOURCE_FILES})

# Link libraries
target_link_libraries(Javitas PRIVATE glad "-framework OpenGL" glfw)

# Include directories
target_include_directories(Javitas PRIVATE external/glad/include external/glm)

# Add GLFW
add_subdirectory(external/glfw)
target_link_libraries(Javitas PRIVATE glfw)

# Add Dear ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_BACKENDS ${IMGUI_DIR}/backends)

# Add Dear ImGui source files
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_BACKENDS}/imgui_impl_glfw.cpp
    ${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp
)

# Include Dear ImGui directories
target_include_directories(Javitas PRIVATE ${IMGUI_DIR} ${IMGUI_BACKENDS})

# Add Dear ImGui sources to the target
target_sources(Javitas PRIVATE ${IMGUI_SOURCES})

# Add a custom command to copy shader files into the build directory
add_custom_command(
    TARGET Javitas
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/src/engine/defaultShader.vs
    ${CMAKE_CURRENT_BINARY_DIR}/defaultShader.vs
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/src/engine/defaultShader.fs
    ${CMAKE_CURRENT_BINARY_DIR}/defaultShader.fs
)

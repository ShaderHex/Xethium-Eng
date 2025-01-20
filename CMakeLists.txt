cmake_minimum_required(VERSION 3.10)
project(xethium)

add_subdirectory(external/raylib)

set(SOURCES
    src/main.cpp
    src/Core/Application.cpp
    src/Core/Renderer.cpp
    src/Camera/Camera.cpp
)

set(RLIMGUI_SOURCES
    external/rlimgui/rlImGui.cpp
)

set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
)

add_executable(xethium
    ${SOURCES}
    ${IMGUI_SOURCES}
    ${RLIMGUI_SOURCES}
)

target_include_directories(xethium PRIVATE
    external/imgui
    external/imgui/backends
    external/rlimgui
    src
)

target_link_libraries(xethium PRIVATE
    raylib
    glfw
    GL  
    m 
    dl
    pthread 
)

set_target_properties(xethium PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)

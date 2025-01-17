cmake_minimum_required(VERSION 3.10)
project(xethium)

add_subdirectory(external/raylib)

set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(xethium
    src/main.cpp
    ${IMGUI_SOURCES}
)

target_include_directories(xethium PRIVATE
    external/imgui
    external/imgui/backends
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

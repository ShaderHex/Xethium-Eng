cmake_minimum_required(VERSION 3.10)
project(xethium)

add_subdirectory(external/raylib)

set(SOURCES
    src/main.cpp
    src/Core/Application.cpp
    src/Core/SceneManager.cpp
    src/Core/FileManager.cpp
    src/Core/UiDgenerator.cpp
    src/Core/EngineState.cpp
    src/Core/GizmoManager.cpp
    src/Renderer/Renderer.cpp
    src/Camera/Camera.cpp
    src/Entities/Rectangle.cpp
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

set(IMGUIZMO_SOURCES
    external/ImGuizmo/ImGuizmo.cpp
)

set(RAYGIZMO_SOURCES
    external/raygizmo/raygizmo.c
)

add_executable(xethium
    ${SOURCES}
    ${IMGUI_SOURCES}
    ${RLIMGUI_SOURCES}
    ${IMGUIZMO_SOURCES}
    ${RAYGIZMO_SOURCES}
)

target_include_directories(xethium PRIVATE
    external/imgui
    external/imgui/backends
    external/rlimgui
    external/ImGuizmo
    external/raygizmo
    src
    src/ThirdParty/nlohmann
)

if(WIN32)
    set(PLATFORM_LIBS opengl32 gdi32)
elseif(UNIX)
    set(PLATFORM_LIBS GL m dl pthread)
endif()

target_link_libraries(xethium PRIVATE
    raylib
    glfw
    ${PLATFORM_LIBS}
)

set_target_properties(xethium PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

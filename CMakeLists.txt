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
    src/Core/Scripting.cpp
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

set(LUA_SOURCES
    external/lua/src/lapi.c
    external/lua/src/lcode.c
    external/lua/src/lctype.c
    external/lua/src/ldebug.c
    external/lua/src/ldo.c
    external/lua/src/ldump.c
    external/lua/src/lfunc.c
    external/lua/src/lgc.c
    external/lua/src/llex.c
    external/lua/src/lmem.c
    external/lua/src/lobject.c
    external/lua/src/lopcodes.c
    external/lua/src/lparser.c
    external/lua/src/lstate.c
    external/lua/src/lstring.c
    external/lua/src/ltable.c
    external/lua/src/ltm.c
    external/lua/src/lundump.c
    external/lua/src/lvm.c
    external/lua/src/lzio.c
    external/lua/src/lauxlib.c
    external/lua/src/lbaselib.c
    external/lua/src/lcorolib.c
    external/lua/src/ldblib.c
    external/lua/src/liolib.c
    external/lua/src/lmathlib.c
    external/lua/src/loslib.c
    external/lua/src/lstrlib.c
    external/lua/src/ltablib.c
    external/lua/src/lutf8lib.c
    external/lua/src/loadlib.c
    external/lua/src/linit.c
)

add_executable(xethium
    ${SOURCES}
    ${IMGUI_SOURCES}
    ${RLIMGUI_SOURCES}
    ${IMGUIZMO_SOURCES}
    ${RAYGIZMO_SOURCES}
    ${LUA_SOURCES}
)

target_include_directories(xethium PRIVATE
    external/imgui
    external/imgui/backends
    external/rlimgui
    external/ImGuizmo
    external/raygizmo
    external/
    external/lua/src
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

#pragma once
#include "defines.h"

namespace XENGINE {
    extern "C" XAPI void Init();
    extern "C" XAPI bool WindowShouldClose();
    extern "C" XAPI void StartDrawing();
    extern "C" XAPI void EndDrawing();
    extern "C" XAPI void CloseWindow();
}

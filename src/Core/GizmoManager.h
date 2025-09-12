#pragma once
#include "raygizmo.h"
#include <raylib.h>

class GizmoManager {
public:
    Transform transform;

    GizmoManager();

    void Draw(int mode = GIZMO_TRANSLATE | GIZMO_ROTATE | GIZMO_SCALE);

    Matrix GetMatrix() const;
};

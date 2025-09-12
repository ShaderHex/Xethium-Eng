#include "GizmoManager.h"

#define RAYGIZMO_IMPLEMENTATION
#include "raygizmo.h"

GizmoManager::GizmoManager() {
    transform = GizmoIdentity();
}

void GizmoManager::Draw(int mode) {
    DrawGizmo3D(mode, &transform);
}

Matrix GizmoManager::GetMatrix() const {
    return GizmoToMatrix(transform);
}

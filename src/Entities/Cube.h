#include "SceneObject.h"

struct RectangleObject : SceneObject {
    Vector3 position;
    Vector3 size;
    Color color;

    std::string GetType() const override { return "Rectangle"; }
};

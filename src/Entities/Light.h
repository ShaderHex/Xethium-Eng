#include "SceneObject.h"

struct LightEntity : SceneObject {
    int type;
    Vector3 position;
    Vector3 target;
    Color color;
    float intensity;
    bool enabled;

    LightEntity() : 
        type(0),
        position{0,2,0},
        target{0,0,0},
        color(WHITE),
        intensity(1.0f),
        enabled(true)
    {
        UiD = 0;
        name = "Light";
    }

    std::string GetType() const override { return "Light"; }
};

#include "CircleObject.h"
#include "imgui.h"

CircleObject::CircleObject(Vector2 pos, float radius, Color col)
    : Entity(pos, col), radius(radius) {}

void CircleObject::Draw() const {
    DrawCircleV(position, radius, color);
}

void CircleObject::DrawInspectorUI() {
    ImGui::Text("Circle: %s", name.c_str());
    ImGui::DragFloat2("Position", (float*)&position);
    ImGui::DragFloat("Radius", &radius, 0.1f, 1.0f, 1000.0f);

    float tempColor[3] = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f
    };
    
    if (ImGui::ColorEdit3("Color", tempColor)) {
        color.r = static_cast<unsigned char>(tempColor[0] * 255);
        color.g = static_cast<unsigned char>(tempColor[1] * 255);
        color.b = static_cast<unsigned char>(tempColor[2] * 255);
    }
}

Rectangle CircleObject::GetBoundingBox() const {
    return {
        position.x - radius,
        position.y - radius,
        radius * 2,
        radius * 2
    };
}
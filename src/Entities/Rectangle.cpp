#include "Rectangle.h"

std::vector<RectangleObject> rectangles;
Color color = {255, 255, 255, 255};

void RectangleObj::CreateRect(std::vector<RectangleObject>& rects) {
    RectangleObject rect;
    rect.UiD = UiDGenerator::GenerateUiD();
    std::string rectName = std::to_string(rect.UiD);
    rect.name = "Rectangle" + rectName;

    rect.position = { 
        (float)GetRandomValue(0, 500), 
        0.0f, 
        (float)GetRandomValue(0, 500) 
    };

    rect.size = {100, 100, 100};

    rect.color = RED;
    rects.push_back(rect);

    std::cout << "UiD: " << rect.UiD << std::endl;
    std::cout << "Pushed back to the memory\n";
}

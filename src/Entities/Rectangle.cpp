#include "Rectangle.h"

std::vector<RectangleObject> rectangles;

void RectangleObj::CreateRect() {
    RectangleObject rect;
    rect.position = { (float)GetRandomValue(0, 500), (float)GetRandomValue(0, 500)};
    rect.size = {100, 100};
    rect.color = (RED);
                
    rectangles.push_back(rect);
    std::cout<<"Pushed back to the memory\n";
}
#pragma once
#include <string>
#include <unordered_map>

namespace Platform {

class Input {
public:
    std::unordered_map<std::string, int> keymap;

    std::unordered_map<int, bool> currentFrame;
    std::unordered_map<int, bool> lastFrame;
    
    void CreateAction(std::string action, int key);
    void Update();
    bool IsActionPressed(std::string action);
    bool IsActionReleased(std::string action);
    bool IsActionHeld(std::string action);
    void CleanUp();


};

}

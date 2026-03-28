#include "input/input.h"
#include "platform/platform.h"
#include <iostream>
#include <glfw/glfw3.h>

namespace Platform {

void Input::CreateAction(std::string action, int key) {
    keymap[action] = key;
    std::cout << "Action key " << action << " has been created successfully" << std::endl;
}

void Input::Update() {
    lastFrame = currentFrame;

    for (auto& km : keymap) {
        currentFrame[km.second] = glfwGetKey(Platform::GetNativeWindow(), km.second);
    }
}

bool Input::IsActionPressed(std::string action) {
    auto it = keymap.find(action);


    if(it == keymap.end()) {
        std::cout<< "keymap " << action << " is not found";
        return false;
    }

    int key = it->second;

    return currentFrame[key] && !lastFrame[key];

}

bool Input::IsActionReleased(std::string action) {
    auto it = keymap.find(action);

    if(it == keymap.end()) {
        std::cout<< "keymap " << action << " is not found";
        return false;
    }

    int key = it->second;

    return !currentFrame[key] && lastFrame[key];
}

bool Input::IsActionHeld(std::string action) {
    auto it = keymap.find(action);

    if(it == keymap.end()) {
        std::cout<< "keymap " << action << " is not found";
        return false;
    }

    int key = it->second;
    return currentFrame[key];
}

}
#include "Scripting.h"
#include <iostream>
#include <fstream>
#include <sstream>

Scripting::Scripting() {

}

Scripting::~Scripting() {

}

void Scripting::init() {
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
}

bool Scripting::runScriptFile(const std::string& path) {
    try {
        lua.script_file(path);
        return true;
    } catch (const sol::error& e) {
        std::cerr << "Lua error (file): " << e.what() << "\n";
        return false;
    }
}

bool Scripting::runScript(const std::string& code) {
    try {
        lua.script(code);
        return true;
    } catch (const sol::error& e) {
        std::cerr << "Lua error: " << e.what() << "\n";
        return false;
    }
}

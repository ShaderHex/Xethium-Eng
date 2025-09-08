#pragma once
#include <sol/sol.hpp>
#include <string>

class Scripting {
public:
    Scripting();
    ~Scripting();

    void init();
    bool runScriptFile(const std::string& path);
    bool runScript(const std::string& code);
    
    sol::state& getState() { return lua; }

private:
    sol::state lua;
};

#pragma once

#include <iostream>
#include <streambuf>
#include <vector>
#include <string>
#include "imgui.h"

class ImGuiLogger : public std::streambuf {
public:
    std::vector<std::string> logs;
    std::ostream out;

    ImGuiLogger() : out(this) {}

    void DrawWindow(const char* title = "Console", bool* p_open = nullptr, int maxLines = 100) {
        ImGui::Begin(title, p_open);

        if (ImGui::Button("Clear Logs")) {
            logs.clear();
        }

        ImGui::BeginChild("LogRegion", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

        int start = (logs.size() > (size_t)maxLines) ? logs.size() - maxLines : 0;
        for (size_t i = start; i < logs.size(); i++) {
            ImGui::TextUnformatted(logs[i].c_str());
        }

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }

protected:
    virtual int_type overflow(int_type c) override {
        if (c == '\n') {
            logs.push_back(buffer);
            buffer.clear();
        } else {
            buffer += static_cast<char>(c);
        }
        return c;
    }

private:
    std::string buffer;
};

inline ImGuiLogger g_Logger;

inline void RedirectCoutToLogger() {
    std::cout.rdbuf(&g_Logger);
}

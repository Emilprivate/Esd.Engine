#pragma once

#include "imgui.h"

namespace Styles {
    inline void DarkTheme() {
        ImGuiStyle* style = &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);

        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.38f, 0.39f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.68f, 1.00f);

        colors[ImGuiCol_Button] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.39f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.68f, 1.00f);

        colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.16f, 0.18f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.39f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.68f, 1.00f);

        colors[ImGuiCol_Text] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
    }
}
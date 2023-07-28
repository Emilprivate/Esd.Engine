#pragma once

#include "imgui.h"

namespace EsdEngineGraphics {
    namespace Styles {
        inline void DefaultStyle(){
            ImGuiStyle& style = ImGui::GetStyle();

            style.WindowPadding = ImVec2(15, 15);
            style.WindowRounding = 5.0f;
            style.FramePadding = ImVec2(6, 6);
            style.FrameRounding = 4.0f;
            style.ItemSpacing = ImVec2(10, 8);
            style.ItemInnerSpacing = ImVec2(8, 6);
            style.IndentSpacing = 25.0f;

            style.ScrollbarSize = 15.0f;
            style.ScrollbarRounding = 9.0f;

            style.GrabMinSize = 5.0f;
            style.GrabRounding = 3.0f;

            style.FrameBorderSize = 0.5f;

            ImVec4 transparent(0.0f, 0.0f, 0.0f, 0.5f);
            ImVec4 white(1.0f, 1.0f, 1.0f, 1.0f);
            ImVec4 grey(0.2f, 0.2f, 0.2f, 1.0f);
            ImVec4 darkGrey(0.15f, 0.15f, 0.15f, 1.0f);
            ImVec4 black(0.0f, 0.0f, 0.0f, 1.0f);

            style.Colors[ImGuiCol_Text] = white;
            style.Colors[ImGuiCol_TextDisabled] = grey;
            style.Colors[ImGuiCol_WindowBg] = transparent;
            style.Colors[ImGuiCol_ChildBg] = darkGrey;
            style.Colors[ImGuiCol_PopupBg] = black;
            style.Colors[ImGuiCol_Border] = transparent;
            style.Colors[ImGuiCol_BorderShadow] = transparent;
            style.Colors[ImGuiCol_FrameBg] = grey;
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.40f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 0.67f);
            style.Colors[ImGuiCol_TitleBg] = darkGrey;
            style.Colors[ImGuiCol_TitleBgActive] = darkGrey;
            style.Colors[ImGuiCol_TitleBgCollapsed] = transparent;
            style.Colors[ImGuiCol_MenuBarBg] = darkGrey;
            style.Colors[ImGuiCol_ScrollbarBg] = darkGrey;
            style.Colors[ImGuiCol_ScrollbarGrab] = grey;
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
            style.Colors[ImGuiCol_CheckMark] = white;
            style.Colors[ImGuiCol_SliderGrab] = grey;
            style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
            style.Colors[ImGuiCol_Button] = grey;
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
            style.Colors[ImGuiCol_Tab] = darkGrey;
            style.Colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
            style.Colors[ImGuiCol_TabActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
            style.Colors[ImGuiCol_TabUnfocused] = darkGrey;
            style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
            style.Colors[ImGuiCol_Header] = darkGrey;
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
        }

        inline void DarkTheme() {
            ImGuiStyle& style = ImGui::GetStyle();

            style.WindowPadding = ImVec2(15, 15);
            style.WindowRounding = 5.0f;
            style.FramePadding = ImVec2(5, 5);
            style.FrameRounding = 4.0f;
            style.ItemSpacing = ImVec2(12, 8);
            style.ItemInnerSpacing = ImVec2(8, 6);
            style.IndentSpacing = 25.0f;

            style.ScrollbarSize = 15.0f;
            style.ScrollbarRounding = 9.0f;

            style.GrabMinSize = 5.0f;
            style.GrabRounding = 3.0f;

            style.FrameBorderSize = 0.5f;

            ImVec4 white(1.0f, 1.0f, 1.0f, 1.0f);
            ImVec4 grey(0.5f, 0.5f, 0.5f, 0.5f);
            ImVec4 black(0.0f, 0.0f, 0.0f, 1.0f);

            style.Colors[ImGuiCol_Text] = white;
            style.Colors[ImGuiCol_TextDisabled] = grey;
            style.Colors[ImGuiCol_WindowBg] = black;
            style.Colors[ImGuiCol_ChildBg] = black;
            style.Colors[ImGuiCol_PopupBg] = black;
            style.Colors[ImGuiCol_Border] = grey;
            style.Colors[ImGuiCol_BorderShadow] = black;
            style.Colors[ImGuiCol_FrameBg] = black;
            style.Colors[ImGuiCol_FrameBgHovered] = grey;
            style.Colors[ImGuiCol_FrameBgActive] = grey;
            style.Colors[ImGuiCol_TitleBg] = black;
            style.Colors[ImGuiCol_TitleBgActive] = black;
            style.Colors[ImGuiCol_TitleBgCollapsed] = black;
            style.Colors[ImGuiCol_MenuBarBg] = black;
            style.Colors[ImGuiCol_ScrollbarBg] = black;
            style.Colors[ImGuiCol_ScrollbarGrab] = grey;
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = white;
            style.Colors[ImGuiCol_ScrollbarGrabActive] = white;
            style.Colors[ImGuiCol_CheckMark] = white;
            style.Colors[ImGuiCol_SliderGrab] = grey;
            style.Colors[ImGuiCol_SliderGrabActive] = white;
            style.Colors[ImGuiCol_Button] = black;
            style.Colors[ImGuiCol_ButtonHovered] = grey;
            style.Colors[ImGuiCol_ButtonActive] = grey;
            style.Colors[ImGuiCol_Tab] = black;
            style.Colors[ImGuiCol_TabHovered] = grey;
            style.Colors[ImGuiCol_TabActive] = grey;
            style.Colors[ImGuiCol_TabUnfocused] = black;
            style.Colors[ImGuiCol_TabUnfocusedActive] = grey;
            style.Colors[ImGuiCol_Header] = black;
            style.Colors[ImGuiCol_HeaderHovered] = grey;
            style.Colors[ImGuiCol_HeaderActive] = grey;
        }

        inline void LightTheme() {
            ImGuiStyle& style = ImGui::GetStyle();

            style.WindowPadding = ImVec2(15, 15);
            style.WindowRounding = 5.0f;
            style.FramePadding = ImVec2(5, 5);
            style.FrameRounding = 4.0f;
            style.ItemSpacing = ImVec2(12, 8);
            style.ItemInnerSpacing = ImVec2(8, 6);
            style.IndentSpacing = 25.0f;

            style.ScrollbarSize = 15.0f;
            style.ScrollbarRounding = 9.0f;

            style.GrabMinSize = 5.0f;
            style.GrabRounding = 3.0f;

            style.FrameBorderSize = 0.5f;

            ImVec4 black(0.0f, 0.0f, 0.0f, 1.0f);
            ImVec4 grey(0.5f, 0.5f, 0.5f, 0.5f);
            ImVec4 white(1.0f, 1.0f, 1.0f, 1.0f);

            style.Colors[ImGuiCol_Text] = black;
            style.Colors[ImGuiCol_TextDisabled] = grey;
            style.Colors[ImGuiCol_WindowBg] = white;
            style.Colors[ImGuiCol_ChildBg] = white;
            style.Colors[ImGuiCol_PopupBg] = white;
            style.Colors[ImGuiCol_Border] = grey;
            style.Colors[ImGuiCol_BorderShadow] = grey;
            style.Colors[ImGuiCol_FrameBg] = white;
            style.Colors[ImGuiCol_FrameBgHovered] = grey;
            style.Colors[ImGuiCol_FrameBgActive] = grey;
            style.Colors[ImGuiCol_TitleBg] = white;
            style.Colors[ImGuiCol_TitleBgActive] = white;
            style.Colors[ImGuiCol_TitleBgCollapsed] = white;
            style.Colors[ImGuiCol_MenuBarBg] = white;
            style.Colors[ImGuiCol_ScrollbarBg] = white;
            style.Colors[ImGuiCol_ScrollbarGrab] = grey;
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = black;
            style.Colors[ImGuiCol_ScrollbarGrabActive] = black;
            style.Colors[ImGuiCol_CheckMark] = black;
            style.Colors[ImGuiCol_SliderGrab] = grey;
            style.Colors[ImGuiCol_SliderGrabActive] = black;
            style.Colors[ImGuiCol_Button] = white;
            style.Colors[ImGuiCol_ButtonHovered] = grey;
            style.Colors[ImGuiCol_ButtonActive] = grey;
            style.Colors[ImGuiCol_Tab] = white;
            style.Colors[ImGuiCol_TabHovered] = grey;
            style.Colors[ImGuiCol_TabActive] = grey;
            style.Colors[ImGuiCol_TabUnfocused] = white;
            style.Colors[ImGuiCol_TabUnfocusedActive] = grey;
            style.Colors[ImGuiCol_Header] = white;
            style.Colors[ImGuiCol_HeaderHovered] = grey;
            style.Colors[ImGuiCol_HeaderActive] = grey;
        }
    }
}
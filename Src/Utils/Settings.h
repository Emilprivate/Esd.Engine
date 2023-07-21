#pragma once

#include "imgui.h"
#include <iostream>
#include <SDL.h>

#include "../Graphics/UI/Styles.h"

class Settings {
private:
    struct Window {
        int width = 1200;
        int height = 800;
        const char* title = "Esd.Engine";

        void SetSize(int newWidth, int newHeight)
        {
            width = newWidth;
            height = newHeight;
        }
    } window;

    struct Renderer {
    private:
        ImVec4 clear_color = {0.0f, 0.0f, 0.0f, 1.00f};

    public:
        ImVec4& GetClearColor()
        {
           return clear_color;
        }
        void SetClearColor(float r, float g, float b, float a)
        {
            clear_color = {r, g, b, a};
        }
    } renderer;

    struct Simulations {
        int subSteps = 8;
        float fps = 60.0f;
        float dt = (1.0f / fps);
    } simulations;

    struct UI {
    private:
        bool ui_initialized = false;
        bool show_demo_window = false;
        bool show_engine_menu = true;

    public:
        enum class ThemeType {Default, Dark, Light} currentTheme = ThemeType::Default;
        bool& ShowDemoWindow() { return show_demo_window; }
        bool& ShowEngineMenu() { return show_engine_menu; }
        bool& UIInitialized() { return ui_initialized; }

    } ui;

public:
    Renderer& GetRenderer() { return renderer; }
    Window& GetWindow() { return window; }
    Simulations& GetSimulations() { return simulations; }
    UI& GetUI() { return ui; }

    static Settings& GetInstance()
    {
        static Settings instance;
        return instance;
    }

    void RenderUI()
    {
        if (!ui.UIInitialized())
        {
            Styles::DefaultStyle();
            ui.UIInitialized() = true;
        }

        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::BeginMenu("Themes"))
            {
                if (ImGui::MenuItem("Default", NULL, ui.currentTheme == UI::ThemeType::Default))
                {
                    ui.currentTheme = UI::ThemeType::Default;
                    Styles::DefaultStyle();
                }
                if (ImGui::MenuItem("Dark", NULL, ui.currentTheme == UI::ThemeType::Dark))
                {
                    ui.currentTheme = UI::ThemeType::Dark;
                    Styles::DarkTheme();
                }
                if (ImGui::MenuItem("Light", NULL, ui.currentTheme == UI::ThemeType::Light))
                {
                    ui.currentTheme = UI::ThemeType::Light;
                    Styles::LightTheme();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Background Color"))
            {
                static ImVec4 color = renderer.GetClearColor();
                ImGui::ColorEdit3("Background Color", (float*)&color);
                renderer.SetClearColor(color.x, color.y, color.z, color.w);
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }
};
#pragma once

#include "imgui.h"
#include <iostream>
#include <SDL.h>

#include "../Graphics/UI/Styles.h"

namespace EsdEngineUtils {
    class Settings {
    private:
        Settings() = default;
        ~Settings() = default;

        struct Application {
            bool exit = false;
        } application;

        struct Window {
            int width = 1200;
            int height = 800;
            const char* title = "Esd.Engine";

            SDL_Window* window{};
            SDL_GLContext gl_context{};

            void SetSize(int newWidth, int newHeight)
            {
                width = newWidth;
                height = newHeight;
            }

            void SetSDLWindow(SDL_Window* newWindow)
            {
                window = newWindow;
            }

            void SetGLContext(SDL_GLContext newContext)
            {
                gl_context = newContext;
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
            float fps = 120;
            float dt = (1.0f / fps);

            bool init_new_sim = true;

            struct Verlet {
                int subSteps = 8;
                float object_spawn_delay = 0.025f;
                float object_spawn_speed = 1000.0f;
                float object_min_radius = 5.0f;
                float object_max_radius = 20.0f;
                float object_spawn_position_x = 0.0f;
                float object_spawn_position_y = 0.0f;
                uint32_t max_objects_count = 500;
                float max_angle = 1.0f;
            } verlet;
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
        Settings(const Settings&) = delete;
        Settings& operator=(const Settings&) = delete;

        Application& GetApplication() { return application; }
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
                EsdEngineGraphics::Styles::DefaultStyle();
                ui.UIInitialized() = true;
            }

            if (ImGui::BeginMenu("Settings"))
            {
                if (ImGui::BeginMenu("Themes"))
                {
                    if (ImGui::MenuItem("Default", nullptr, ui.currentTheme == UI::ThemeType::Default))
                    {
                        ui.currentTheme = UI::ThemeType::Default;
                        EsdEngineGraphics::Styles::DefaultStyle();
                    }
                    if (ImGui::MenuItem("Dark", nullptr, ui.currentTheme == UI::ThemeType::Dark))
                    {
                        ui.currentTheme = UI::ThemeType::Dark;
                        EsdEngineGraphics::Styles::DarkTheme();
                    }
                    if (ImGui::MenuItem("Light", nullptr, ui.currentTheme == UI::ThemeType::Light))
                    {
                        ui.currentTheme = UI::ThemeType::Light;
                        EsdEngineGraphics::Styles::LightTheme();
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
}

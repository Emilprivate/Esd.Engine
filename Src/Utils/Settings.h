#pragma once

#include "imgui.h"
#include <iostream>
#include <SDL.h>

class Settings {
private:

    struct Window {
        int width = 800;
        int height = 600;
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
        int subSteps = 1;
        float fps = 60.0f;
        float dt = 1.0f / fps;
    } simulations;

public:
    Renderer& GetRenderer() { return renderer; }
    Window& GetWindow() { return window; }
    Simulations& GetSimulations() { return simulations; }

    static Settings& GetInstance()
    {
        static Settings instance;
        return instance;
    }
};
#pragma once
#include "../../Utils/Settings.h"
#include "Graphics/UI/UI.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>


class Renderer {
public:
    Renderer();
    ~Renderer();

    void Initialize();
    void Render(UI& ui, SimulationsHandler& simHandler);
    void Cleanup();

private:
    Settings& settings = Settings::GetInstance();

    void RenderImGui();
};
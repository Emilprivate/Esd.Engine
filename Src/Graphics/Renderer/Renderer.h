#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

#include "../../Utils/Settings.h"
#include "../UI/UI.h"
#include "../../Simulations/SimulationsHandler.h"

class Renderer
{
public:
    explicit Renderer();
    ~Renderer();

    void Initialize();
    void Render();
    void Cleanup();

    bool ShouldClose();

private:
    bool isInitialized;

    Settings& settings = Settings::GetInstance();
    SimulationsHandler& simulationsHandler = SimulationsHandler::GetInstance();
    UI ui;
};
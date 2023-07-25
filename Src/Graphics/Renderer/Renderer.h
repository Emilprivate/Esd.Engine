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
    Renderer() { Initialize(); }
    ~Renderer() { Cleanup(); }

    static void Initialize();
    static void Run();
    static bool ShouldClose();
    static void Cleanup();
};
#pragma once
#include "../../Utils/Settings.h"
#include "Graphics/UI/UI.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

namespace EsdEngineGraphics {
    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void Initialize();
        void Render(UI& ui, EsdEngineSimulations::SimulationsHandler& simHandler);
        void Cleanup();

    private:
        EsdEngineUtils::Settings& settings = EsdEngineUtils::Settings::GetInstance();

        void RenderImGui();
    };
}

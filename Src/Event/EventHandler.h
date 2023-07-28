#pragma once

#include "SDL.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream>

#include "../Graphics/Window/Window.h"

namespace EsdEngineEvent {
    class EventHandler {
    public:
        EventHandler();
        ~EventHandler();

        void HandleEvents(EsdEngineGraphics::Window& window);

    private:
        bool CheckForExitRequest();

        SDL_Event event{};
        EsdEngineUtils::Settings& settings = EsdEngineUtils::Settings::GetInstance();
    };
}



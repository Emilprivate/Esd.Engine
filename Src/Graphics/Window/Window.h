#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include "../../Utils/Settings.h"

namespace EsdEngineGraphics {
    class Window {
    public:
        Window();
        ~Window();

        void Initialize();
        void HandleEvents(SDL_Event& event);
        void Cleanup();

    private:
        EsdEngineUtils::Settings& settings = EsdEngineUtils::Settings::GetInstance();
    };
}
#pragma once
#include "../Graphics/Window/Window.h"
#include "../Graphics/Renderer/Renderer.h"
#include "../Graphics/UI/UI.h"

#include "../Event/EventHandler.h"

#include "../Simulations/SimulationsHandler.h"

#include "../Utils/Settings.h"

#include <memory>
#include <SDL.h>

namespace EsdEngineCore {
    class Engine {
    public:
        Engine();
        ~Engine();

        Settings& settings = Settings::GetInstance();

        std::unique_ptr<EsdEngineGraphics::Window> window;
        std::unique_ptr<EsdEngineGraphics::Renderer> renderer;
        std::unique_ptr<EsdEngineEvent::EventHandler> eventHandler;
        std::unique_ptr<EsdEngineSimulations::SimulationsHandler> simHandler;
        std::unique_ptr<EsdEngineGraphics::UI> ui;
    };

    void Run();
}



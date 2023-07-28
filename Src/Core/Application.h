#pragma once
#include "../Graphics/Window/Window.h"
#include "../Graphics/Renderer/Renderer.h"
#include "../Graphics/UI/UI.h"

#include "../Event/EventHandler.h"

#include "../Simulations/SimulationsHandler.h"

#include "../Utils/Settings.h"

#include <memory>
#include <SDL.h>

class Application {
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<EventHandler> eventHandler;
    std::unique_ptr<SimulationsHandler> simHandler;
    std::unique_ptr<UI> ui;

    Settings& settings = Settings::GetInstance();
public:
    Application();
    ~Application();

    void Run();
};

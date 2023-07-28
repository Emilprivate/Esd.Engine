#include "Engine.h"

namespace EsdEngineCore {
    Engine::Engine() {
        std::cout << "Initializing window" << std::endl;
        window = std::make_unique<EsdEngineGraphics::Window>();
        window->Initialize();

        std::cout << "Initializing renderer" << std::endl;
        renderer = std::make_unique<EsdEngineGraphics::Renderer>();
        renderer->Initialize();

        std::cout << "Initializing simulations handler" << std::endl;
        simHandler = std::make_unique<EsdEngineSimulations::SimulationsHandler>();
        simHandler->Initialize();

        std::cout << "Initializing event handler" << std::endl;
        eventHandler = std::make_unique<EsdEngineEvent::EventHandler>();

        std::cout << "Initializing user interface" << std::endl;
        ui = std::make_unique<EsdEngineGraphics::UI>(*simHandler);
    }

    Engine::~Engine() {
        std::cout << "Cleaning up renderer" << std::endl;
        renderer->Cleanup();

        std::cout << "Cleaning up window" << std::endl;
        window->Cleanup();

        std::cout << "Cleaning up simulations handler" << std::endl;
        simHandler->Cleanup();

        std::cout << "Cleaning up event handler" << std::endl;
        eventHandler->Cleanup();
    }

    void Run() {
        Engine engine;

        Uint32 frameDelay;

        while (engine.settings.GetApplication().exit == false) {
            Uint32 frameStart = SDL_GetTicks();

            engine.eventHandler->HandleEvents(*engine.window);
            engine.simHandler->Update();
            engine.renderer->Render(*engine.ui, *engine.simHandler);

            frameDelay = 1000.0f / engine.settings.GetSimulations().fps;
            Uint32 frameTime = SDL_GetTicks() - frameStart;

            if (frameTime < frameDelay)
            {
                SDL_Delay(frameDelay - frameTime);
            }
        }
    }
}
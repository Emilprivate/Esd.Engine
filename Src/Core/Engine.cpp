#include "Engine.h"

namespace EsdEngineCore {
    // I'm intentionally controlling the initialization and cleanup order of the engine's main components
    // to avoid any potential issues with the order of destruction of the components and to have a better
    // control over the initialization and cleanup process during the lifetime of the engine.

    /*
     * The order of initialization and cleanup is as follows:
     * 1. Window
     * 2. Renderer
     * 3. Simulations handler
     * 4. Event handler
     * 5. User interface
     */
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

        std::cout << "Engine initialization complete" << std::endl;
    }

    /**
     * The order of cleanup is the reverse of the order of initialization.
     * 1. Window
     * 2. Renderer
     * 3. Simulations handler
     * 4. Event handler
     */
    Engine::~Engine() {
        std::cout << "Cleaning up event handler" << std::endl;
        eventHandler->Cleanup();

        std::cout << "Cleaning up simulations handler" << std::endl;
        simHandler->Cleanup();

        std::cout << "Cleaning up renderer" << std::endl;
        renderer->Cleanup();

        std::cout << "Cleaning up window" << std::endl;
        window->Cleanup();

        std::cout << "Engine cleanup complete" << std::endl;
    }

    /**
     * The main loop of the engine.
     * The loop is controlled by the exit flag in the application settings.
     * The loop is also controlled by the frame delay to ensure that the engine
     * runs at the desired frame rate.
     */
    void Run() {
        // Create an instance of the engine
        Engine engine;

        // The frame delay is the time in milliseconds that the engine should wait
        Uint32 frameDelay;

        // The main loop of the engine
        while (engine.settings.GetApplication().exit == false) {
            // Get the current time in milliseconds
            Uint32 frameStart = SDL_GetTicks();

            // Handle events, update the simulations and render the scene
            engine.eventHandler->HandleEvents(*engine.window);
            engine.simHandler->Update();
            engine.renderer->Render(*engine.ui, *engine.simHandler);

            // Calculate the frame delay and wait for the remaining time
            frameDelay = 1000.0f / engine.settings.GetSimulations().fps;
            Uint32 frameTime = SDL_GetTicks() - frameStart;

            if (frameTime < frameDelay)
            {
                SDL_Delay(frameDelay - frameTime);
            }
        }
    }
}
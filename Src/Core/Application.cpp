#include "Application.h"

Application::Application() {
    std::cout << "Initializing window" << std::endl;
    window = std::make_unique<Window>();
    window->Initialize();

    std::cout << "Initializing renderer" << std::endl;
    renderer = std::make_unique<Renderer>();
    renderer->Initialize();

    std::cout << "Initializing simulations handler" << std::endl;
    simHandler = std::make_unique<SimulationsHandler>();
    simHandler->Initialize();

    std::cout << "Initializing event handler" << std::endl;
    eventHandler = std::make_unique<EventHandler>();

    std::cout << "Initializing user interface" << std::endl;
    ui = std::make_unique<UI>(*simHandler);
}

Application::~Application() = default;

void Application::Run() {
    Uint32 frameDelay;

    while (settings.GetApplication().exit == false) {
        Uint32 frameStart = SDL_GetTicks();

        eventHandler->HandleEvents(*window);
        simHandler->Update();
        renderer->Render(*ui, *simHandler);

        frameDelay = 1000.0f / settings.GetSimulations().fps;
        Uint32 frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < frameDelay)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

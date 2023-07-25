#include "EngineCore.h"

Uint32 EngineCore::CalculateFrameDelay()
{
    auto& settings = Settings::GetInstance().GetSimulations();
    return 1000 / settings.fps;
}

void EngineCore::EngineLoop()
{
    const Uint32 frameDelay = CalculateFrameDelay();

    while (!Renderer::ShouldClose())
    {
        Uint32 frameStart = SDL_GetTicks();

        Renderer::Run();

        Uint32 frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < frameDelay)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void EngineCore::Run()
{
    Renderer::Initialize();
    EngineLoop();
}

#include "EngineCore.h"

void EngineCore::Run()
{
    Renderer renderer = Renderer();
    Settings& settings = Settings::GetInstance();
    renderer.Initialize();

    while (!renderer.ShouldClose())
    {
        Uint32 frameStart = SDL_GetTicks();

        renderer.Render();

        Uint32 frameEnd = SDL_GetTicks();
        Uint32 frameTime = frameEnd - frameStart;

        if (frameTime < 1000 / settings.GetSimulations().fps)
        {
            SDL_Delay((1000 / settings.GetSimulations().fps) - frameTime);
        }
    }

    renderer.Cleanup();
}

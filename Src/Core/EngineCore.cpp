#include "EngineCore.h"

void EngineCore::Run() {
    Renderer renderer = Renderer();

    renderer.Initialize();

    while (!renderer.ShouldClose())
    {
        Uint32 frameStart = SDL_GetTicks(); // Start of frame

        renderer.Render();

        Uint32 frameEnd = SDL_GetTicks(); // End of frame
        Uint32 frameTime = frameEnd - frameStart; // Frame time

        if (frameTime < 1000 / 60) { // Delay to achieve the desired frame rate (60 fps)
            SDL_Delay((1000 / 60) - frameTime);
        }
    }

    renderer.Cleanup();
}

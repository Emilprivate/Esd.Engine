#include "EngineCore.h"

void EngineCore::Run() {
    {
        Renderer renderer = Renderer();

        renderer.Initialize();

        while (!renderer.ShouldClose())
        {
            renderer.Render();
        }

        renderer.Cleanup();
    }
}
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include "../../Utils/Settings.h"

class Window {
public:
    Window();
    ~Window();

    void Initialize();
    void HandleEvents(SDL_Event& event);
    void Cleanup();

private:
    Settings& settings = Settings::GetInstance();

    SDL_Window* m_window;
    SDL_GLContext m_gl_context;
};

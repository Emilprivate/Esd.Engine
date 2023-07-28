#pragma once

#include "SDL.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream>

#include "../Graphics/Window/Window.h"

class EventHandler {
public:
    EventHandler();
    ~EventHandler();

    void HandleEvents(Window& window);

private:
    bool CheckForExitRequest();

    SDL_Event event{};
    Settings& settings = Settings::GetInstance();
};


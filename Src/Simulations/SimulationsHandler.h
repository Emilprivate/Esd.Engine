#pragma once

#include "../Utils/Settings.h"
//TODO: Add steps to the simulation handler
class SimulationsHandler {
private:
    Uint32 previousTicks = SDL_GetTicks();
    Uint32 lag = 0;
    const Uint32 MS_PER_UPDATE = 1000 / 60;

public:
    SimulationsHandler() = default;
    ~SimulationsHandler() = default;

    void Initialize();
    void Render();
    void Update();
};

#include "SimulationsHandler.h"
#include "Simulations/TestSimulations/BouncingCircle.h"

//TODO: Add steps to the simulation handler

void SimulationsHandler::Initialize()
{
    BouncingCircleSimulation::Initialize();

}

void SimulationsHandler::Update() {
    Uint32 currentTicks = SDL_GetTicks();
    Uint32 elapsedTicks = currentTicks - previousTicks;
    previousTicks = currentTicks;
    lag += elapsedTicks;

    while (lag >= MS_PER_UPDATE) {
        float currentTime = previousTicks / 1000.0f;
        BouncingCircleSimulation::Update(currentTime);
        lag -= MS_PER_UPDATE;
    }
}

void SimulationsHandler::Render()
{
    BouncingCircleSimulation::Render();
}
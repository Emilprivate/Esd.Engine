#include "SimulationsHandler.h"

SimulationsHandler& SimulationsHandler::GetInstance()
{
    static SimulationsHandler instance;
    return instance;
}

void SimulationsHandler::SetCurrentSimulation(int index)
{
    currentSimulation = simulations[index];
}

void SimulationsHandler::Initialize()
{
    if (currentSimulation)
    {
        currentSimulation->Initialize();
    }
}

void SimulationsHandler::Run()
{
    if (currentSimulation)
    {
        currentSimulation->Run();
    }
}

void SimulationsHandler::RenderUI()
{
    if (currentSimulation)
    {
        currentSimulation->RenderUI();
    }
}

#include "SimulationsHandler.h"

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

void SimulationsHandler::Update(){
    if (currentSimulation)
    {
        currentSimulation->Update();
    }
}

void SimulationsHandler::Render(){
    if (currentSimulation)
    {
        currentSimulation->Render();
    }
}

void SimulationsHandler::RenderUI()
{
    if (currentSimulation)
    {
        currentSimulation->RenderUI();
    }
}

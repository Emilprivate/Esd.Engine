#include "SimulationsHandler.h"

namespace EsdEngineSimulations {
    SimulationsHandler::SimulationsHandler() = default;
    SimulationsHandler::~SimulationsHandler() = default;

    void SimulationsHandler::SetCurrentSimulation(int index)
    {
        currentSimulation = simulations[index];
    }

    void SimulationsHandler::Initialize()
    {
        AddSimulation(new VerletSimulationManager());
        AddSimulation(new EditorSimulationManager());

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

    void SimulationsHandler::Cleanup() {
        for (auto simulation : simulations)
            delete simulation;
    }

}
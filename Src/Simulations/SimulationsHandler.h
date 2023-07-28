#pragma once

#include "Simulations/VerletSimulation/Verlet.h"
#include "Simulations/EngineEditor/Editor.h"

#include "../Utils/Settings.h"
#include "SimulationBase.h"

class SimulationsHandler {
private:
    std::vector<SimulationBase*> simulations;
    SimulationBase* currentSimulation = nullptr;

    SimulationsHandler()
    {
        AddSimulation(new EditorSimulationManager());
        AddSimulation(new VerletSimulationManager());
    }

    ~SimulationsHandler()
    {
        for (auto simulation: simulations)
            delete simulation;
    }

    void AddSimulation(SimulationBase* simulation) {
        simulations.push_back(simulation);
        if (simulations.size() == 1) {
            SetCurrentSimulation(0);
        }
    }

public:
    SimulationsHandler(const SimulationsHandler&) = delete;
    SimulationsHandler& operator=(const SimulationsHandler&) = delete;

    static SimulationsHandler& GetInstance();

    std::vector<std::string> GetSimulationNames() {
        std::vector<std::string> names;
        for (const auto& simulation : simulations)
        {
            names.push_back(simulation->GetName());
        }
        return names;
    }

    void SetCurrentSimulation(int index);
    void Initialize();
    void Run();
    void RenderUI();
};
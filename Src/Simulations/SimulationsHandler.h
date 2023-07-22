#pragma once

#include "Simulations/TestSimulations/Verlet.h"
#include "Simulations/TestSimulations/Editor.h"

#include "../Utils/Settings.h"
#include "SimulationBase.h"

class SimulationsHandler {
private:
    Settings& settings = Settings::GetInstance();
    std::vector<SimulationBase*> simulations;
    SimulationBase* currentSimulation = nullptr;

    SimulationsHandler() {
        AddSimulation(new VerletSimulationManager());
        AddSimulation(new EditorSimulationManager());
    }

    ~SimulationsHandler() {
        for (auto simulation : simulations) {
            delete simulation;
        }
    }

public:
    SimulationsHandler(const SimulationsHandler&) = delete;
    SimulationsHandler& operator=(const SimulationsHandler&) = delete;

    static SimulationsHandler& GetInstance() {
        static SimulationsHandler instance;
        return instance;
    }

    void AddSimulation(SimulationBase* simulation) {
        simulations.push_back(simulation);
        if (simulations.size() == 1) {
            SetCurrentSimulation(0);
        }
    }

    void SetCurrentSimulation(int index);
    void Initialize();
    void Render();
    void Update();
    void RenderUI();

    std::vector<std::string> GetSimulationNames();
};
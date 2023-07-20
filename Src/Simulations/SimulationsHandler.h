#pragma once

#include "Simulations/TestSimulations/Circle.h"
#include "Simulations/TestSimulations/Verlet.h"

#include "../Utils/Settings.h"
#include "SimulationBase.h"

class SimulationsHandler {
private:
    Settings& settings = Settings::GetInstance();
    std::vector<SimulationBase*> simulations;
    SimulationBase* currentSimulation = nullptr;  // Pointer to the current simulation

    SimulationsHandler() {
        // Instantiate the simulation objects and add them to the simulations vector
        AddSimulation(new VerletSimulationManager());
        AddSimulation(new CircleSimulationManager());

        // Add other simulations here...
    }

    ~SimulationsHandler() {
        // Don't forget to deallocate the simulations to avoid memory leaks
        for (auto simulation : simulations) {
            delete simulation;
        }
    }

public:
    // Deleted functions
    SimulationsHandler(const SimulationsHandler&) = delete;
    SimulationsHandler& operator=(const SimulationsHandler&) = delete;

    // Static method for getting the instance
    static SimulationsHandler& GetInstance() {
        static SimulationsHandler instance;  // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    void AddSimulation(SimulationBase* simulation) {
        simulations.push_back(simulation);
        if (simulations.size() == 1) {
            SetCurrentSimulation(0);
        }
    }

    void SetCurrentSimulation(int index);  // New method for setting the current simulation
    void Initialize();
    void Render();
    void Update();
    void RenderUI();  // New method for rendering the UI of the current simulation

    std::vector<std::string> GetSimulationNames();
};
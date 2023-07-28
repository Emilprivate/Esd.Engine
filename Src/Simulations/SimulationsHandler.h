#pragma once

#include "Simulations/VerletSimulation/Verlet.h"
#include "Simulations/EngineEditor/Editor.h"

#include "../Utils/Settings.h"
#include "SimulationBase.h"

namespace EsdEngineSimulations {
    class SimulationsHandler {
    private:
        std::vector<SimulationBase*> simulations;
        SimulationBase* currentSimulation = nullptr;

        void AddSimulation(SimulationBase* simulation) {
            simulations.push_back(simulation);
            if (simulations.size() == 1) {
                SetCurrentSimulation(0);
            }
        }

    public:
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
        void Update();
        void Render();
        void RenderUI();
    };
}
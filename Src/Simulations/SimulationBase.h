#pragma once

namespace EsdEngineSimulations {
    class SimulationBase {
    public:
        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void RenderUI() = 0;

        virtual std::string GetName() = 0;
    };
}


#pragma once

class SimulationBase {
public:
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void RenderUI() = 0;

    // New method to get the name of the simulation
    virtual std::string GetName() = 0;
};



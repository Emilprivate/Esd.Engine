#pragma once

class SimulationBase {
public:
    virtual void Initialize() = 0;
    virtual void Run() = 0;
    virtual void RenderUI() = 0;

    virtual std::string GetName() = 0;
};
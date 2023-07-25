#pragma once

#include "../Graphics/Renderer/Renderer.h"

class EngineCore
{
public:
    static void Run();

    EngineCore(const EngineCore&) = delete;
    EngineCore& operator = (const EngineCore&) = delete;

private:
    EngineCore() = default;
    ~EngineCore() = default;

    static void EngineLoop();
    static Uint32 CalculateFrameDelay();
};

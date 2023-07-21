#pragma once

#include <imgui.h>

#include "../../Utils/Settings.h"
#include "../../Simulations/SimulationsHandler.h"
#include "Styles.h"

class UI {
public:
    UI() = default;
    ~UI() = default;

    void Render();

private:
    Settings& settings = Settings::GetInstance();
    SimulationsHandler& simulationsHandler = SimulationsHandler::GetInstance();

    void MainMenuBar();
};

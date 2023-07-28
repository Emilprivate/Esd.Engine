#pragma once

#include <imgui.h>

#include "../../Utils/Settings.h"
#include "../../Simulations/SimulationsHandler.h"
#include "Styles.h"

class UI {
public:
    explicit UI(SimulationsHandler& handler);
    void Render();

private:
    void MainMenuBar();
    SimulationsHandler& simHandler;
    Settings& settings = Settings::GetInstance();
};

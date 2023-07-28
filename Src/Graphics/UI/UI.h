#pragma once

#include <imgui.h>

#include "../../Utils/Settings.h"
#include "../../Simulations/SimulationsHandler.h"
#include "Styles.h"

namespace EsdEngineGraphics {
    class UI {
    public:
        explicit UI(EsdEngineSimulations::SimulationsHandler& handler);
        void Render();

    private:
        void MainMenuBar();
        EsdEngineSimulations::SimulationsHandler& simHandler;
        EsdEngineUtils::Settings& settings = EsdEngineUtils::Settings::GetInstance();
    };
}


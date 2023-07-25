#pragma once

#include <imgui.h>

#include "../../Utils/Settings.h"
#include "../../Simulations/SimulationsHandler.h"
#include "Styles.h"

class UI {
public:
    UI() = default;
    ~UI() = default;

    static void Render();

private:
    static void MainMenuBar();
};

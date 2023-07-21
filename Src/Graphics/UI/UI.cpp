#include "UI.h"

void UI::Render() {
    // Starting with the Demo window
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Begin("Esd.Engine Menu", reinterpret_cast<bool *>(true), ImGuiWindowFlags_NoScrollbar);

    // Reusing the Settings object
    auto& renderer = settings.GetRenderer();

    // For Checkboxes
    ImGui::Checkbox("Demo Window", &show_demo_window);

    // Theming
    Styles::DefaultStyle();

    // Background Color
    static ImVec4 color = renderer.GetClearColor();
    if(ImGui::ColorEdit3("Background Color", (float*)&color)) {
        renderer.SetClearColor(color.x, color.y, color.z, color.w);
    }

    // Simulation Dropdown
    static int selectedSimulation = 0;
    auto simulationNames = simulationsHandler.GetSimulationNames();

    std::vector<const char*> cstrs;
    std::transform(simulationNames.begin(), simulationNames.end(), std::back_inserter(cstrs),
                   [](const std::string& str) {
                       return str.c_str();
                   });

    if (ImGui::Combo("Simulation", &selectedSimulation, cstrs.data(), cstrs.size())) {
        simulationsHandler.SetCurrentSimulation(selectedSimulation);
    }

    // Sliders
    auto& simSettings = settings.GetSimulations();
    ImGui::SliderInt("Substeps", &simSettings.subSteps, 1, 10);
    ImGui::SliderFloat("FPS", &simSettings.fps, 1.0f, 120.0f);

    // FPS info
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // Child Window
    ImGui::BeginChild("Simulation Settings", ImVec2(0, 0), true);
    simulationsHandler.RenderUI();
    ImGui::EndChild();



    // Closing the window
    ImGui::End();
}
#include "UI.h"

void UI::Render() {
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    {
        ImGui::Begin("Esd.Engine Menu");
        ImGui::Checkbox("Demo Window", &show_demo_window);

        Styles::DarkTheme();

        static ImVec4 color = settings.GetRenderer().GetClearColor();
        ImGui::ColorEdit3("Background Color", (float*)&color);
        settings.GetRenderer().SetClearColor(color.x, color.y, color.z, color.w);

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

        ImGui::SliderInt("Substeps", &settings.GetSimulations().subSteps, 1, 10);
        ImGui::SliderFloat("FPS", &settings.GetSimulations().fps, 1.0f, 120.0f);


        simulationsHandler.RenderUI();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}

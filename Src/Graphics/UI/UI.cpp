#include "UI.h"

void UI::MainMenuBar()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Engine"))
    {
        if (ImGui::MenuItem("Exit"))
        {
            SDL_Event quitEvent;
            quitEvent.type = SDL_QUIT;
            SDL_PushEvent(&quitEvent);
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View"))
    {
        ImGui::MenuItem("Esd.Engine Menu", nullptr, &settings.GetUI().ShowEngineMenu());
        ImGui::EndMenu();
    }

    settings.RenderUI();
    ImGui::EndMainMenuBar();
}

void UI::Render()
{
    MainMenuBar();

    if (settings.GetUI().ShowEngineMenu()) {
        ImGui::Begin("Esd.Engine Menu", &settings.GetUI().ShowEngineMenu(), ImGuiWindowFlags_NoScrollbar);

        if (settings.GetUI().ShowDemoWindow()) {
            ImGui::ShowDemoWindow(&settings.GetUI().ShowDemoWindow());
        }

        static int selectedSimulation = 0;
        auto simulationNames = simulationsHandler.GetSimulationNames();

        std::vector<const char*> cstrs;
        std::transform(simulationNames.begin(), simulationNames.end(), std::back_inserter(cstrs),
                       [](const std::string& str) {
                           return str.c_str();
                       });

        if (ImGui::Combo("Simulation", &selectedSimulation, cstrs.data(), cstrs.size()))
        {
            simulationsHandler.SetCurrentSimulation(selectedSimulation);
        }

        auto& simSettings = settings.GetSimulations();
        ImGui::SliderInt("Substeps", &simSettings.subSteps, 1, 10);
        ImGui::SliderFloat("FPS", &simSettings.fps, 1.0f, 120.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::BeginChild("Simulation Settings", ImVec2(0, 0), true);
        simulationsHandler.RenderUI();
        ImGui::EndChild();

        ImGui::End();
    }
}
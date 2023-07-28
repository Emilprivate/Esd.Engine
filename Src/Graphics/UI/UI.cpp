#include "UI.h"

UI::UI(SimulationsHandler& simHandler) : simHandler(simHandler) {}

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
        ImGui::SetNextWindowSize(ImVec2(400, 400));

        ImGui::Begin("Esd.Engine Menu", &settings.GetUI().ShowEngineMenu(), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

        if (settings.GetUI().ShowDemoWindow()) {
            ImGui::ShowDemoWindow(&settings.GetUI().ShowDemoWindow());
        }

        static int selectedSimulation = 0;
        auto simulationNames = simHandler.GetSimulationNames();

        std::vector<const char*> cstrs;
        std::transform(simulationNames.begin(), simulationNames.end(), std::back_inserter(cstrs),
                       [](const std::string& str) {
                           return str.c_str();
                       });

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));
        if (ImGui::Combo("Simulation", &selectedSimulation, cstrs.data(), cstrs.size())) {
            simHandler.SetCurrentSimulation(selectedSimulation);
            settings.GetSimulations().init_new_sim = true;
        }

        ImGui::SliderFloat("FPS", &settings.GetSimulations().fps, 1.0f, 120.0f);

        ImGui::Separator();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Separator();

        ImGui::BeginChild("Simulation Settings", ImVec2(0, 0), true);
        simHandler.RenderUI();
        ImGui::EndChild();

        ImGui::PopStyleVar();

        ImGui::End();
    }
}

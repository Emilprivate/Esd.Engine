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
        ImGui::MenuItem("Esd.Engine Menu", nullptr, &Settings::GetInstance().GetUI().ShowEngineMenu());
        ImGui::EndMenu();
    }

    Settings::GetInstance().RenderUI();

    ImGui::EndMainMenuBar();
}

void UI::Render()
{
    MainMenuBar();

    if (Settings::GetInstance().GetUI().ShowEngineMenu()) {
        ImGui::SetNextWindowSize(ImVec2(400, 400));

        ImGui::Begin("Esd.2DEngine Menu", &Settings::GetInstance().GetUI().ShowEngineMenu(), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

        if (Settings::GetInstance().GetUI().ShowDemoWindow()) {
            ImGui::ShowDemoWindow(&Settings::GetInstance().GetUI().ShowDemoWindow());
        }

        static int selectedSimulation = 0;
        auto simulationNames = SimulationsHandler::GetInstance().GetSimulationNames();

        std::vector<const char*> cstrs;
        std::transform(simulationNames.begin(), simulationNames.end(), std::back_inserter(cstrs),
                       [](const std::string& str) {
                           return str.c_str();
                       });

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));
        if (ImGui::Combo("Simulation", &selectedSimulation, cstrs.data(), cstrs.size())) {
            SimulationsHandler::GetInstance().SetCurrentSimulation(selectedSimulation);
            Settings::GetInstance().GetSimulations().init_new_sim = true;
        }

        auto& simSettings = Settings::GetInstance().GetSimulations();
        ImGui::SliderFloat("FPS", &simSettings.fps, 1.0f, 120.0f);
        std::cout << simSettings.fps << std::endl;

        ImGui::Separator();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Separator();

        ImGui::BeginChild("Simulation Settings", ImVec2(0, 0), true);
        SimulationsHandler::GetInstance().RenderUI();
        ImGui::EndChild();

        ImGui::PopStyleVar();

        ImGui::End();
    }
}

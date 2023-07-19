#include "UI.h"

void UI::Render() {

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    {
        ImGui::Begin("Esd.Engine Menu");
        ImGui::Checkbox("Demo Window", &show_demo_window);

        static ImVec4 color = settings.GetRenderer().GetClearColor();
        ImGui::ColorEdit3("Background Color", (float*)&color);
        settings.GetRenderer().SetClearColor(color.x, color.y, color.z, color.w);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}
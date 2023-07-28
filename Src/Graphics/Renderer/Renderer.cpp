#include "Renderer.h"

namespace EsdEngineGraphics {
    Renderer::Renderer() = default;
    Renderer::~Renderer() = default;

    void Renderer::Initialize() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui_ImplSDL2_InitForOpenGL(settings.GetWindow().window, settings.GetWindow().gl_context);
        ImGui_ImplOpenGL2_Init();
    }

    void Renderer::Render(UI& ui, EsdEngineSimulations::SimulationsHandler& simHandler) {
        ImVec4 clear_color = settings.GetRenderer().GetClearColor();

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(settings.GetWindow().window);
        ImGui::NewFrame();

        int realWidth, realHeight;
        SDL_GL_GetDrawableSize(settings.GetWindow().window, &realWidth, &realHeight);
        settings.GetWindow().SetSize(realWidth, realHeight);

        glViewport(0, 0, settings.GetWindow().width, settings.GetWindow().height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, settings.GetWindow().width, settings.GetWindow().height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        simHandler.Render();
        ui.Render();

        RenderImGui();

        SDL_GL_SwapWindow(settings.GetWindow().window);
    }

    void Renderer::RenderImGui() {
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

    void Renderer::Cleanup() {
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
}



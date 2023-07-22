#include "Renderer.h"

Renderer::Renderer() :
        isInitialized(false){}

Renderer::~Renderer()
{
    Cleanup();
}

void Renderer::Initialize()
{
    if (isInitialized)
        return;

    simulationsHandler.Initialize();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return;
    }

#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    auto window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    auto window = SDL_CreateWindow(settings.GetWindow().title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings.GetWindow().width, settings.GetWindow().height, window_flags);
    settings.GetWindow().SetSDLWindow(window);

    auto gl_context = SDL_GL_CreateContext(window);
    settings.GetWindow().SetGLContext(gl_context);

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();

    isInitialized = true;
}

bool Renderer::ShouldClose()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
        {
            return true;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(settings.GetWindow().window))
        {
            return true;
        }
    }

    return false;
}

void Renderer::Render()
{
    if (!isInitialized)
        return;

    ImVec4 clear_color = settings.GetRenderer().GetClearColor();

    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
        {
            return;
        }

        if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                settings.GetWindow().SetSize(event.window.data1, event.window.data2);

                int realWidth, realHeight;

                SDL_GL_GetDrawableSize(settings.GetWindow().window, &realWidth, &realHeight);

                glViewport(0, 0, realWidth, realHeight);
            }
        }

    }

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

    simulationsHandler.Update();
    simulationsHandler.Render();

    ui.Render();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(settings.GetWindow().window);
}

void Renderer::Cleanup()
{
    if (!isInitialized)
        return;

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(settings.GetWindow().gl_context);
    SDL_DestroyWindow(settings.GetWindow().window);
    SDL_Quit();

    isInitialized = false;
}
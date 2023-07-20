#include "Renderer.h"

Renderer::Renderer() :
        window(nullptr),
        gl_context(nullptr),
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

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    auto window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    window = SDL_CreateWindow(settings.GetWindow().title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings.GetWindow().width, settings.GetWindow().height, window_flags);
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ); // Enable linear filtering

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
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
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
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

    // Clear the buffer at the beginning
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    // Process SDL events
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
                //Set viewport for the OpenGL rendering
                settings.GetWindow().SetSize(event.window.data1, event.window.data2);

                std::cout << "Window width: " << settings.GetWindow().width << " Window height: " << settings.GetWindow().height << std::endl;

                int realWidth, realHeight;

                SDL_GL_GetDrawableSize(window, &realWidth, &realHeight);

                glViewport(0, 0, realWidth, realHeight); // update the viewport size
            }
        }

    }

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    // Update the window size in the settings
    int realWidth, realHeight;
    SDL_GL_GetDrawableSize(window, &realWidth, &realHeight);
    settings.GetWindow().SetSize(realWidth, realHeight);

    glViewport(0, 0, settings.GetWindow().width, settings.GetWindow().height);

    // Set up orthographic projection and render circles
    glMatrixMode(GL_PROJECTION); // Switch to altering the projection matrix
    glLoadIdentity(); // Reset the projection matrix
    glOrtho(0, settings.GetWindow().width, settings.GetWindow().height, 0, -1, 1); // Set the orthographic projection
    glMatrixMode(GL_MODELVIEW); // Switch back to the model-view matrix
    glLoadIdentity(); // Reset the model-view matrix

    simulationsHandler.Update();
    simulationsHandler.Render();

    ui.Render(); // Render UI

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
}

void Renderer::Cleanup()
{
    if (!isInitialized)
        return;

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    isInitialized = false;
}
#include "Window.h"

namespace EsdEngineGraphics {
    Window::Window() = default;
    Window::~Window() = default;

    void Window::Initialize() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            std::cerr << "Error: " << SDL_GetError() << std::endl;
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

        auto l_window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

        auto l_window = SDL_CreateWindow(settings.GetWindow().title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings.GetWindow().width, settings.GetWindow().height, l_window_flags);
        settings.GetWindow().SetSDLWindow(l_window);

        auto l_gl_context = SDL_GL_CreateContext(l_window);
        settings.GetWindow().SetGLContext(l_gl_context);

        SDL_GL_MakeCurrent(l_window, l_gl_context);
        SDL_GL_SetSwapInterval(1);
        SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    }

    void Window::HandleEvents(SDL_Event& event) {
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

    void Window::Cleanup() {
        SDL_GL_DeleteContext(settings.GetWindow().gl_context);
        SDL_DestroyWindow(settings.GetWindow().window);
        SDL_Quit();
    }
}


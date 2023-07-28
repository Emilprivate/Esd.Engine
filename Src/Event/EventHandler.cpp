#include "EventHandler.h"

namespace EsdEngineEvent {
    EventHandler::EventHandler() = default;
    EventHandler::~EventHandler() = default;

    void EventHandler::HandleEvents(EsdEngineGraphics::Window &window) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            window.HandleEvents(event);

            if (CheckForExitRequest()) {
                settings.GetApplication().exit = true;
            } else {
                settings.GetApplication().exit = false;
            }
        }
    }

    bool EventHandler::CheckForExitRequest() {
        if (event.type == SDL_QUIT) {
            return true;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(settings.GetWindow().window)) {
            return true;
        }

        return false;
    }

    void EventHandler::Cleanup(){}
}

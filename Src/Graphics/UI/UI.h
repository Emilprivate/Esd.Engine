#include "imgui.h"
#include "../../Utils/Settings.h"

class UI {
public:
    UI() = default;
    ~UI() = default;

    void Render();

private:
    bool show_demo_window{}, show_another_window{};

    Settings& settings = Settings::GetInstance();
};

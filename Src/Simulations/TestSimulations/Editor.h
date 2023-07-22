#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <vector>
#include "../../Utils/Settings.h"
#include "../../Utils/Utils.h"
#include "../SimulationBase.h"

class Wall {
public:
    bool operator==(const Wall& other) const {
        return start == other.start && end == other.end;
    }

    Vector2<float> start, end;
    Color color;

    Wall(const Vector2<float>& start, const Vector2<float>& end, const Color& color)
            : start(start), end(end), color(color) {}

    void Draw() {
        glColor3f(color.red, color.green, color.blue);
        glBegin(GL_LINES);
        glVertex2f(start.x, start.y);
        glVertex2f(end.x, end.y);
        glEnd();
    }
};

class PhysicsController {
private:
    std::vector<Wall> walls;

public:
    void AddWall(const Wall& wall) {
        walls.push_back(wall);
    }

    void RemoveWall(Wall& wall) {
        walls.erase(std::remove(walls.begin(), walls.end(), wall), walls.end());
    }

    void DrawWalls() {
        for (auto& wall : walls) {
            wall.Draw();
        }
    }

    Wall* GetWallUnderCursor(Vector2<float> cursorPosition, float maxDistance) {
        for (auto& wall : walls) {
            float distance = PointToSegmentDistance(cursorPosition, wall.start, wall.end);
            if (distance < maxDistance) {
                return &wall;
            }
        }
        return nullptr;
    }
};

class EditorSimulationManager : public SimulationBase {
private:
    Settings& settings = Settings::GetInstance();
    PhysicsController physicsController;
    bool simulationRunning = false;
    Uint32 last_spawn_time = 0;
    Wall* currentWall = nullptr;
    bool isWallDrawingMode = false;
    Color currentColor = {1.0f, 1.0f, 1.0f};
    bool isEraserMode = false;
    float eraserRadius = 10.0f;


    void DrawObject() {
        physicsController.DrawWalls();
    }

    void DrawEraser(const Vector2<float>& position) {
        const int num_segments = 100;
        const float angle_increment = 2.0f * M_PI / static_cast<float>(num_segments);

        glColor4f(1.0f, 0.0f, 0.0f, 0.5f);  // Red color
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < num_segments; ++i) {
            float theta = angle_increment * static_cast<float>(i);
            float dx = eraserRadius * std::cos(theta);
            float dy = eraserRadius * std::sin(theta);
            glVertex2f(position.x + dx, position.y + dy);
        }
        glEnd();
    }

public:
    EditorSimulationManager() = default;

    void Initialize() override {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    void Update() override {
        int x, y;
        SDL_GetMouseState(&x, &y);
        Vector2<float> currentPosition = {static_cast<float>(x), static_cast<float>(y)};

        if (isEraserMode && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))) {
            Wall* wall = physicsController.GetWallUnderCursor(currentPosition, eraserRadius);
            if (wall) {
                physicsController.RemoveWall(*wall);
            }
        } else if (isWallDrawingMode && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))) {
            if (currentWall) {
                currentWall->end = currentPosition;
                physicsController.AddWall(*currentWall);
                delete currentWall;
            }
            currentWall = new Wall(currentPosition, currentPosition, currentColor);
        } else if (currentWall) {
            delete currentWall;
            currentWall = nullptr;
        }
    }


    void Render() override {
        DrawObject();

        if (isEraserMode) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            DrawEraser(Vector2<float>{static_cast<float>(mouseX), static_cast<float>(mouseY)});
        }

        if (currentWall) {
            currentWall->Draw();
        }
    }

    void RenderUI() override {
        if (ImGui::Checkbox("Wall Drawing Mode", &isWallDrawingMode)) {
            isEraserMode = !isWallDrawingMode;
        }

        if (ImGui::Checkbox("Eraser Mode", &isEraserMode)) {
            isWallDrawingMode = !isEraserMode;
        }

        if (isEraserMode) {
            ImGui::DragFloat("Eraser Radius", &eraserRadius, 0.1f, 1.0f, 100.0f);
        } else if (isWallDrawingMode) {
            ImGui::ColorEdit3("Wall Color", (float*)&currentColor); // color picker
        }
    }

    std::string GetName() override {
        return "Physics Editor";
    }
};
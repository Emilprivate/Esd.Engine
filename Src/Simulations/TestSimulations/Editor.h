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
    Vector2<float> start, end;
    Color color;

    bool operator==(const Wall& other) const {
        return start == other.start && end == other.end;
    }

    Wall(const Vector2<float>& start, const Vector2<float>& end, const Color& color)
            : start(start), end(end), color(color) {}

    void Rotate(float angleInRadians) {
        float cs = std::cos(angleInRadians);
        float sn = std::sin(angleInRadians);

        Vector2<float> center = (start + end) / 2;

        start -= center;
        end -= center;

        float startXNew = start.x * cs - start.y * sn;
        float startYNew = start.x * sn + start.y * cs;

        float endXNew = end.x * cs - end.y * sn;
        float endYNew = end.x * sn + end.y * cs;

        start = Vector2<float>(startXNew, startYNew) + center;
        end = Vector2<float>(endXNew, endYNew) + center;
    }

    void Draw() {
        glColor3f(color.red, color.green, color.blue);
        glBegin(GL_LINES);
        glVertex2f(start.x, start.y);
        glVertex2f(end.x, end.y);
        glEnd();
    }
};

class Shape {
public:
    std::vector<Wall> walls;
};

class PhysicsController {
public:
    void AddWall(const Wall& wall) {
        // If there are no shapes or the last shape is closed, start a new shape.
        if (shapes.empty() || IsShapeClosed(shapes.back())) {
            shapes.push_back(Shape());
        }
        // Add the wall to the current shape.
        shapes.back().walls.push_back(wall);
    }

    void RemoveWall(Wall& wall) {
        for (auto& shape : shapes) {
            shape.walls.erase(std::remove(shape.walls.begin(), shape.walls.end(), wall), shape.walls.end());
            if (shape.walls.empty()) {
                shapes.erase(std::remove_if(shapes.begin(), shapes.end(), [](const Shape& shape){ return shape.walls.empty(); }), shapes.end());
                break;
            }
        }
    }

    void DrawWalls() {
        for (auto& shape : shapes) {
            for (auto& wall : shape.walls) {
                wall.Draw();
            }
        }
    }

    Wall* GetWallUnderCursor(Vector2<float> cursorPosition, float maxDistance) {
        for (auto& shape : shapes) {
            for (auto& wall : shape.walls) {
                float distance = PointToSegmentDistance(cursorPosition, wall.start, wall.end);
                if (distance < maxDistance) {
                    return &wall;
                }
            }
        }
        return nullptr;
    }

    void RotateWallUnderCursor(Vector2<float> cursorPosition, float maxDistance, float angleInRadians) {
        Wall* wall = GetWallUnderCursor(cursorPosition, maxDistance);
        if (wall) {
            wall->Rotate(angleInRadians);
        }
    }

private:
    std::vector<Shape> shapes;

    bool IsShapeClosed(const Shape& shape) {
        if (shape.walls.size() < 3) {  // A closed shape needs at least 3 walls.
            return false;
        }
        const Wall& firstWall = shape.walls.front();
        const Wall& lastWall = shape.walls.back();
        // Check if the start point of the first wall is near the end point of the last wall, or vice versa.
        return std::sqrt(std::pow((firstWall.start.x - lastWall.end.x), 2) + std::pow((firstWall.start.y - lastWall.end.y), 2)) < WALL_CONNECTION_EPSILON
               || std::sqrt(std::pow((lastWall.start.x - firstWall.end.x), 2) + std::pow((lastWall.start.y - firstWall.end.y), 2)) < WALL_CONNECTION_EPSILON;
    }

    static constexpr float WALL_CONNECTION_EPSILON = 0.1f;  // Adjust this as needed.
};


class EditorSimulationManager : public SimulationBase {
private:
    Settings& settings = Settings::GetInstance();
    PhysicsController physicsController;
    Wall* currentWall = nullptr;
    bool isWallDrawingMode = false;
    Color currentColor = {1.0f, 1.0f, 1.0f};
    bool isEraserMode = false;
    float eraserRadius = 10.0f;
    bool isRotateMode = false;
    float rotationAngle = 0.0f;


    void DrawObject() {
        physicsController.DrawWalls();
    }

    void DrawEraser(const Vector2<float>& position) {
        const int num_segments = 100;
        const float angle_increment = 2.0f * M_PI / static_cast<float>(num_segments);

        glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
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

    void Initialize() override
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    void Update() override
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT && isRotateMode)
                    {
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        Vector2<float> currentPosition = {static_cast<float>(x), static_cast<float>(y)};
                        physicsController.RotateWallUnderCursor(currentPosition, 10.0f, rotationAngle);
                    }
                    break;
            }
        }

        int x, y;
        SDL_GetMouseState(&x, &y);
        Vector2<float> currentPosition = {static_cast<float>(x), static_cast<float>(y)};

        if (isRotateMode && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)))
        {
            physicsController.RotateWallUnderCursor(currentPosition, 10.0f, rotationAngle);
        } else if (currentWall)
        {
            currentWall->Rotate(rotationAngle);
        }

        if (isEraserMode && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)))
        {
            Wall* wall = physicsController.GetWallUnderCursor(currentPosition, eraserRadius);
            if (wall)
            {
                physicsController.RemoveWall(*wall);
            }
        }

        if (isWallDrawingMode && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)))
        {
            if (ImGui::IsAnyItemActive()) return;
            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) return;

            if (currentWall)
            {
                currentWall->end = currentPosition;
                physicsController.AddWall(*currentWall);
                delete currentWall;
            }
            currentWall = new Wall(currentPosition, currentPosition, currentColor);
        }

        if (!isWallDrawingMode && currentWall)
        {
            delete currentWall;
            currentWall = nullptr;
        }
    }

    void Render() override
    {

        DrawObject();

        if (isEraserMode)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            DrawEraser(Vector2<float>{static_cast<float>(mouseX), static_cast<float>(mouseY)});
        }

        if (currentWall)
        {
            currentWall->Draw();
        }
    }

    void RenderUI() override
    {
        if (ImGui::Button("Clear"))
        {
            physicsController = PhysicsController();
        }

        // ===================== WALL DRAWING =====================

        if (ImGui::Checkbox("Wall Drawing Mode", &isWallDrawingMode))
        {
            isEraserMode = !isWallDrawingMode;
            isRotateMode = !isWallDrawingMode;
        }

        if (isWallDrawingMode)
        {
            ImGui::ColorEdit3("Wall Color", (float*)&currentColor);
        }

        // ===================== ERASER =====================

        if (ImGui::Checkbox("Eraser Mode", &isEraserMode))
        {
            isWallDrawingMode = !isEraserMode;
            isRotateMode = !isEraserMode;
        }

        if (isEraserMode)
        {
            ImGui::DragFloat("Eraser Radius", &eraserRadius, 0.1f, 1.0f, 100.0f);
        }

        // ===================== ROTATE =====================

        if (ImGui::Checkbox("Rotate Mode", &isRotateMode))
        {
            isWallDrawingMode = !isRotateMode;
            isEraserMode = !isRotateMode;
        }

        if (isRotateMode)
        {
            ImGui::SliderFloat("Rotation Angle", &rotationAngle, -M_PI, M_PI);
        }
    }

    std::string GetName() override
    {
        return "Physics Editor";
    }
};
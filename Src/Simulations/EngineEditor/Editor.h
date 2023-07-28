#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <utility>
#include <vector>
#include "Utils/Settings.h"
#include "Utils/Utils.h"
#include "Simulations/SimulationBase.h"

class Shape {
public:
    virtual ~Shape() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
};

class Polygon : public Shape {
private:
    std::vector<Vector2<float>> vertices;
    Color color;
public:
    Polygon(std::vector<Vector2<float>> vertices) : vertices(std::move(vertices)) {
        color = { 0.0f, 0.0f, 1.0f };
    }
    void Update() override {
        // Update the polygon
    }
    void Draw() override {
        glBegin(GL_POLYGON);
        glColor3f(color.red, color.green, color.blue);
        for (auto& vertex : vertices) {
            glVertex2f(vertex.x, vertex.y);
        }
        glEnd();
    }
};

class Circle : public Shape {
private:
    Vector2<float> position;
    float radius;
    Color color;
public:
    Circle(Vector2<float> position, float radius) : position(position), radius(radius) {
        color = { 1.0f, 0.0f, 0.0f };
    }
    void Update() override {
        // Update the circle
    }
    void Draw() override {
        const float DEG2RAD = 3.14159f / 180;
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(color.red, color.green, color.blue);
        glVertex2f(position.x, position.y);
        for (int i = 0; i <= 360; i++) {
            float degInRad = i * DEG2RAD;
            glVertex2f(position.x + cos(degInRad) * radius, position.y + sin(degInRad) * radius);
        }
        glEnd();
    }
};

class Rectangle : public Shape {
private:
    Vector2<float> position;
    Vector2<float> size;
    Color color;
public:
    Rectangle(Vector2<float> position, Vector2<float> size) : position(position), size(size) {
        color = { 0.0f, 1.0f, 0.0f };
    }
    void Update() override {
        // Update the rectangle
    }
    void Draw() override {
        glBegin(GL_QUADS);
        glColor3f(color.red, color.green, color.blue);
        glVertex2f(position.x, position.y);
        glVertex2f(position.x + size.x, position.y);
        glVertex2f(position.x + size.x, position.y + size.y);
        glVertex2f(position.x, position.y + size.y);
        glEnd();
    }
};

// The ShapesManager class is responsible for managing all the shapes
class ShapesManager {
private:
    std::vector<std::unique_ptr<Shape>> shapes;
    std::string currentShape = "Circle";
    std::vector<std::string> shapesNames;
public:
    ShapesManager() = default;
    ~ShapesManager() = default;

    void AddShape(std::unique_ptr<Shape> shape) {
        shapes.push_back(std::move(shape));
    }

    void InitializeShapes()
    {
        shapesNames.emplace_back("Circle");
        shapesNames.emplace_back("Rectangle");
    }

    void UpdateShapes() {
        for (auto& shape : shapes) {
            shape->Update();
        }
    }

    void DrawShapes() {
        for (auto& shape : shapes) {
            shape->Draw();
        }
    }

    void RenderUI(){
        if (ImGui::BeginCombo("Shapes", currentShape.c_str()))
        {
            for (auto& shapeName : shapesNames)
            {
                bool isSelected = (currentShape == shapeName);
                if (ImGui::Selectable(shapeName.c_str(), isSelected))
                {
                    currentShape = shapeName;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::IsMouseClicked(0))
        {
            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemActive() || ImGui::IsAnyItemFocused() || ImGui::IsAnyItemHovered())
                return;

            ImVec2 mousePos = ImGui::GetMousePos();
            Vector2<float> mousePosFloat = { mousePos.x, mousePos.y };

            if (currentShape == "Circle")
            {
                AddShape(std::make_unique<Circle>(mousePosFloat, 50.0f));
            }
            else if (currentShape == "Rectangle")
            {
                AddShape(std::make_unique<Rectangle>(mousePosFloat, Vector2<float>(100.0f, 100.0f)));
            }
        }
    }
};

// The EditorSimulationManager class is responsible for running the simulation
class EditorSimulationManager : public EsdEngineSimulations::SimulationBase {
private:
    Settings& settings = Settings::GetInstance();
    ShapesManager shapesManager;

public:
    EditorSimulationManager() = default;
    ~EditorSimulationManager() = default;

    void Initialize() override {
        shapesManager.InitializeShapes();
    }

    void Update() override
    {
        shapesManager.UpdateShapes();
    }

    void Render() override
    {
        shapesManager.DrawShapes();
    }

    void RenderUI() override
    {
        shapesManager.RenderUI();
    }

    std::string GetName() override {
        return "Physics Editor";
    }
};
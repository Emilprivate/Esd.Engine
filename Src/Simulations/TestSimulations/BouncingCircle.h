#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <random>
#include <vector>

#include "../../Utils/Settings.h"
#include "../SimulationBase.h"

class Circle {
private:
    float x, y;
    float radius;
    float vx, vy;
    float mass;
    float windowWidth, windowHeight;
    float red, green, blue;
    float gravity;
    float friction;
    Settings& settings = Settings::GetInstance();

    void CheckCollisionWithWindow()
    {
        if (x - radius < 0) {
            vx = std::abs(vx) * friction;
            x = radius;
        } else if (x + radius > windowWidth) {
            vx = -std::abs(vx) * friction;
            x = windowWidth - radius;
        }
        if (y - radius < 0) {
            vy = std::abs(vy) * friction;
            y = radius;
        } else if (y + radius > windowHeight) {
            vy = -std::abs(vy) * friction;
            y = windowHeight - radius;
        }
    }

    void CheckCollisionWithCircle(Circle& other)
    {
        float dx = other.x - x;
        float dy = other.y - y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < radius + other.radius) {
            // Calculate collision response
            float nx = dx / distance;
            float ny = dy / distance;
            float tx = -ny;
            float ty = nx;

            // Project velocities onto the collision normal and tangent
            float v1n = nx * vx + ny * vy;
            float v1t = tx * vx + ty * vy;
            float v2n = nx * other.vx + ny * other.vy;
            float v2t = tx * other.vx + ty * other.vy;

            // Calculate new normal velocities after collision (assuming perfectly elastic collision)
            float v1n_after = (v1n * (mass - other.mass) + 2 * other.mass * v2n) / (mass + other.mass);
            float v2n_after = (v2n * (other.mass - mass) + 2 * mass * v1n) / (mass + other.mass);

            // Convert velocities back to x and y components
            float v1n_after_x = v1n_after * nx;
            float v1n_after_y = v1n_after * ny;
            float v1t_after_x = v1t * tx;
            float v1t_after_y = v1t * ty;
            float v2n_after_x = v2n_after * nx;
            float v2n_after_y = v2n_after * ny;
            float v2t_after_x = v2t * tx;
            float v2t_after_y = v2t * ty;

            // Update velocities
            vx = v1n_after_x + v1t_after_x;
            vy = v1n_after_y + v1t_after_y;
            other.vx = v2n_after_x + v2t_after_x;
            other.vy = v2n_after_y + v2t_after_y;
        }
    }

public:
    Circle(float x, float y, float radius, float vx, float vy, float mass, float friction, float gravity, float windowWidth, float windowHeight) :
           x(x), y(y), radius(radius), vx(vx), vy(vy), mass(mass), friction(friction), gravity(gravity), windowWidth(windowWidth), windowHeight(windowHeight)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        red = dist(gen);
        green = dist(gen);
        blue = dist(gen);
    }

    void Update()
    {
        x += vx;
        y += vy;
        vy += gravity;

        CheckCollisionWithWindow();
    }

    void Render() const
    {
        glBegin(GL_TRIANGLE_FAN);

        glColor3f(red, green, blue);

        glVertex2f(x, y);

        const int num_segments = 100;

        for (int i = 0; i <= num_segments; i++) {
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
            float dx = radius * cosf(theta);
            float dy = radius * sinf(theta);
            glVertex2f(x + dx, y + dy);
        }

        glEnd();
    }
};

class BouncingCircleSimulationManager : public SimulationBase{
private:
    bool simulationRunning = false;

    Settings& settings = Settings::GetInstance();

    std::vector<Circle> Circles;
    std::random_device rd {};
    std::mt19937 gen {rd()};

    // Local settings for this class
    int numCircles;
    float spawnDelay;
    float accumulatedTime;
    float distRadiusMin, distRadiusMax;
    float distSpeedXMin, distSpeedXMax;
    float distMassMin, distMassMax;

    std::uniform_real_distribution<float> distRadius;
    std::uniform_real_distribution<float> distSpeedX;
    std::uniform_real_distribution<float> distMass;

    void SpawnCircle()
    {
        float radius = distRadius(gen);
        float x = settings.GetWindow().width / 2.0f;
        float y = settings.GetWindow().height / 2.0f;
        float speedX = distSpeedX(gen);
        float speedY = 0.0f;
        float mass = distMass(gen);
        float gravity = 0.8f;
        float restitution = 0.95f;

        Circles.emplace_back(x, y, radius, speedX, speedY, mass, restitution, gravity, settings.GetWindow().width, settings.GetWindow().height);
    }

public:
    explicit BouncingCircleSimulationManager() : gen(rd()) {}

    void Initialize() override
    {
        SpawnCircle();
    }

    void Update() override
    {
        if (!simulationRunning) {
            // If the simulation isn't running, don't do anything
            return;
        }

        accumulatedTime += settings.GetSimulations().dt;

        if (accumulatedTime >= spawnDelay && Circles.size() < numCircles) {
            SpawnCircle();
            accumulatedTime = 0.0f;
        }

        // Here is where we implement the sub-steps.
        // The simulation step will be run as many times as specified by subSteps,
        // but with a smaller time delta.
        for (int i = 0; i < settings.GetSimulations().subSteps; ++i) {
            for (auto& circle : Circles) {
                circle.Update();
            }
        }
    }

    void Render() override
    {
        for (const auto& circle : Circles) {
            circle.Render();
        }
    }

    void Reset() {
        Circles.clear();  // Clear the existing circles
        accumulatedTime = 0.0f;  // Reset the accumulated time
        simulationRunning = false;  // Stop the simulation
    }

    void RenderUI() override
    {
        // Adjust the local settings via UI
        if (ImGui::SliderInt("Number of Circles", &numCircles, 1, 1000)) {}

        if (ImGui::SliderFloat("Spawn Delay", &spawnDelay, 0.1f, 10.0f)) {}

        static float distRadius[2] = {distRadiusMin, distRadiusMax};
        if (ImGui::SliderFloat2("Radius Range", distRadius, 10.0f, 100.0f)) {
            distRadiusMin = distRadius[0];
            distRadiusMax = distRadius[1];
            this->distRadius = std::uniform_real_distribution<float>{distRadiusMin, distRadiusMax};
        }

        if (ImGui::SliderInt("Sub-Steps", &settings.GetSimulations().subSteps, 1, 10)) {}

        if (ImGui::Button("Start Simulation")) {
            simulationRunning = true;
        }

        if (ImGui::Button("Stop Simulation")) {
            simulationRunning = false;
        }

        if (ImGui::Button("Reset Simulation")) {
            Reset();
        }
    }

    std::string GetName() override {
        return "Bouncing Circle";
    }
};


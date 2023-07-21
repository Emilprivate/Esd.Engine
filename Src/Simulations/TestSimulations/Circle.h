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
    float prevX, prevY;
    float radius;
    float vx, vy;
    float accelerationX, accelerationY;
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

public:
    Circle(float x, float y, float radius, float vx, float vy, float mass, float friction, float gravity, float windowWidth, float windowHeight) :
            x(x), y(y), radius(radius), vx(vx), vy(vy), mass(mass), friction(friction), gravity(gravity), windowWidth(windowWidth), windowHeight(windowHeight)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        // Get current time with SDL
        Uint32 ticks = SDL_GetTicks();

        // Use a sine wave to generate RGB values
        red = std::pow(std::sin(ticks), 2.0f);
        green = std::pow(std::sin(ticks + 0.33f * 2.0f * 3.14159265359f), 2.0f);
        blue = std::pow(std::sin(ticks + 0.66f * 2.0f * 3.14159265359f), 2.0f);
    }

    void Update()
    {
        const float MIN_VELOCITY = 0.01f;  // Minimum velocity, adjust as needed

        vy += gravity; // Apply gravity
        x += vx; // Update position
        y += vy;
        vx *= friction; // Apply friction
        vy *= friction;

        // Check if the velocity is below the threshold, and if so, set it to zero
        if (std::abs(vx) < MIN_VELOCITY) vx = 0.0f;
        if (std::abs(vy) < MIN_VELOCITY) vy = 0.0f;

        CheckCollisionWithWindow();
    }

    void Render() const
    {
        glBegin(GL_TRIANGLE_FAN);

        glColor3f(red, green, blue);

        glVertex2f(x, y);

        const int num_segments = 500;

        for (int i = 0; i <= num_segments; i++) {
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
            float dx = radius * cosf(theta);
            float dy = radius * sinf(theta);
            glVertex2f(x + dx, y + dy);
        }

        glEnd();
    }

    void CheckCollisionWithCircle(Circle& other)
    {
        float dx = other.x - x;
        float dy = other.y - y;
        float distance = std::sqrt(dx * dx + dy * dy);
        float overlap = radius + other.radius - distance;

        if (overlap > 0) {
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

            // Positional correction to avoid overlap
            float correction = overlap / (mass + other.mass);
            correction += 0.01f;  // add a small constant to ensure minimum movement
            x -= nx * correction * other.mass;
            y -= ny * correction * other.mass;
            other.x += nx * correction * mass;
            other.y += ny * correction * mass;
        }
    }

};

class CircleSimulationManager : public SimulationBase{
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
        float x = radius;
        float y = radius;
        float speedX = distSpeedX(gen);
        float speedY = 0.0f;
        float mass = distMass(gen);
        float gravity = 0.8f;
        float friction = 0.98f;

        Circles.emplace_back(x, y, radius, speedX, speedY, mass, friction, gravity, settings.GetWindow().width, settings.GetWindow().height);
    }

public:
    explicit CircleSimulationManager() : gen(rd()) {}

    void Initialize() override
    {
    }

    void Update() override
    {
        if (!simulationRunning)
            return;

        accumulatedTime += settings.GetSimulations().dt;

        if (accumulatedTime >= spawnDelay && Circles.size() < numCircles) {
            SpawnCircle();
            accumulatedTime = 0.0f;
        }

        for (int i = 0; i < settings.GetSimulations().subSteps; ++i) {
            for (auto& circle : Circles) {
                circle.Update();
            }

            // Checking collision between each pair of circles
            for (size_t j = 0; j < Circles.size(); ++j) {
                for (size_t k = j + 1; k < Circles.size(); ++k) {
                    Circles[j].CheckCollisionWithCircle(Circles[k]);
                }
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

        if (ImGui::SliderFloat("Spawn Delay", &spawnDelay, 0.0f, 10.0f)) {}

        static float distRadius[2] = {distRadiusMin, distRadiusMax};
        if (ImGui::SliderFloat2("Radius Range", distRadius, 5.0f, 100.0f)) {
            distRadiusMin = distRadius[0];
            distRadiusMax = distRadius[1];
            this->distRadius = std::uniform_real_distribution<float>{distRadiusMin, distRadiusMax};
        }

        static float distSpeedX[2] = {distSpeedXMin, distSpeedXMax};
        if (ImGui::SliderFloat2("Speed Range", distSpeedX, 0.0f, 100.0f)) {
            distSpeedXMin = distSpeedX[0];
            distSpeedXMax = distSpeedX[1];
            this->distSpeedX = std::uniform_real_distribution<float>{distSpeedXMin, distSpeedXMax};
        }

        static float distMass[2] = {distMassMin, distMassMax};
        if (ImGui::SliderFloat2("Mass Range", distMass, 0.1f, 10.0f)) {
            distMassMin = distMass[0];
            distMassMax = distMass[1];
            this->distMass = std::uniform_real_distribution<float>{distMassMin, distMassMax};
        }

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
        return "Circle Simulation";
    }
};


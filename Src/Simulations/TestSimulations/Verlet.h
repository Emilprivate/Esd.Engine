#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <random>
#include <vector>

#include "../../Utils/Settings.h"
#include "../../Utils/Utils.h"
#include "../SimulationBase.h"

struct VerletObject
{
    Vector2<float> position;
    Vector2<float> position_last;
    Vector2<float> acceleration;
    float          radius = 10.0f;
    Color          color  = Color{1.0f, 0.0f, 0.0f};

    VerletObject() = default;

    VerletObject(Vector2<float> position_, float radius_):
        position{position_},
        position_last{position_},
        acceleration{0.0f, 0.0f},
        radius{radius_}
    {}

    void update(float dt)
    {
        const Vector2<float> displacement = position - position_last;

        position_last = position;
        position      = position + displacement + acceleration * (dt * dt);

        acceleration = Vector2<float>{};
    }

    void accelerate(Vector2<float> a)
    {
        acceleration += a;
    }

    void setVelocity(Vector2<float> v, float dt)
    {
        position_last = position - (v * dt);
    }

    void addVelocity(Vector2<float> v, float dt)
    {
        position_last -= v * dt;
    }

    [[maybe_unused]] [[nodiscard]]
    Vector2<float> getVelocity(float dt) const
    {
        if (dt != 0.0f) {
            return (position - position_last) / dt;
        } else {
            std::cerr << "Warning: Division by zero detected at [getVelocity]. Returning default Vector2 value." << std::endl;
        }

        return Vector2<float>{};
    }
};

class Solver
{
public:
    Solver() = default;
    ~Solver() = default;

    VerletObject& addObject(Vector2<float> position, float radius)
    {
        return m_objects.emplace_back(position, radius);
    }

    VerletObject& getObject(size_t index)
    {
        if (index < m_objects.size()) {
            return m_objects[index];
        } else {
            throw std::out_of_range("Index out of range in getObject()");
        }
    }

    void update()
    {
        m_time += m_frame_dt;
        const float step_dt = getStepDt();
        for (uint32_t i{m_sub_steps}; i--;) {
            applyGravity();
            checkCollisions();
            applyConstraint();
            updateObjects(step_dt);
        }
    }

    void setSimulationUpdateRate(uint32_t rate)
    {
        m_frame_dt = 1.0f / static_cast<float>(rate);
    }

    void setSubStepsCount(uint32_t sub_steps)
    {
        m_sub_steps = sub_steps;
    }

    void setObjectVelocity(VerletObject& object, Vector2<float> v) const
    {
        object.setVelocity(v, getStepDt());
    }

    [[nodiscard]]
    const std::vector<VerletObject>& getObjects() const
    {
        return m_objects;
    }

    [[nodiscard]]
    uint64_t getObjectsCount() const
    {
        return m_objects.size();
    }

    [[nodiscard]]
    float getTime() const
    {
        return m_time;
    }

    [[nodiscard]]
    float getStepDt() const
    {
        if (m_sub_steps != 0)
        {
            return m_frame_dt / static_cast<float>(m_sub_steps);
        } else {
            std::cerr << "Warning: Division by zero detected at [getStepDt]. Returning default value 0." << std::endl;
        }

        return 0.0f;
    }

private:
    Settings&                 m_settings           = Settings::GetInstance();

    uint32_t                  m_sub_steps          = 1;
    Vector2<float>            m_gravity            = {0.0f, 1000.0f};
    std::vector<VerletObject> m_objects;
    float                     m_time               = 0.0f;
    float                     m_frame_dt           = 0.0f;

    void applyGravity()
    {
        for (auto& obj : m_objects)
        {
            obj.accelerate(m_gravity);
        }
    }

    void checkCollisions()
    {
        const float    response_coef = 0.75f;
        const uint64_t objects_count = m_objects.size();

        for (uint64_t i{0}; i < objects_count; ++i)
        {
            VerletObject& object_1 = m_objects[i];

            for (uint64_t k{i + 1}; k < objects_count; ++k)
            {
                VerletObject&      object_2 = m_objects[k];
                const Vector2<float> v        = object_1.position - object_2.position;
                const float        dist2    = v.x * v.x + v.y * v.y;
                const float        min_dist = object_1.radius + object_2.radius;

                if (dist2 < min_dist * min_dist)
                {
                    const float        dist  = sqrt(dist2);
                    if (dist != 0.0f)
                    {
                        const Vector2<float> n     = v / dist;
                        const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
                        const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
                        const float delta        = 0.5f * response_coef * (dist - min_dist);
                        object_1.position -= n * (mass_ratio_2 * delta);
                        object_2.position += n * (mass_ratio_1 * delta);
                    } else {
                        std::cerr << "Warning: Division by zero detected at [dist -> checkCollisions]. Skipping operation." << std::endl;
                    }
                }
            }
        }
    }

    void applyConstraint()
    {
        const auto& windowSize = m_settings.GetWindow();
        for (auto& obj : m_objects) {
            if (obj.position.x - obj.radius < 0) obj.position.x = obj.radius;
            if (obj.position.y - obj.radius < 0) obj.position.y = obj.radius;
            if (obj.position.x + obj.radius > (float)windowSize.width) obj.position.x = (float)windowSize.width - obj.radius;
            if (obj.position.y + obj.radius > (float)windowSize.height) obj.position.y = (float)windowSize.height - obj.radius;
        }
    }

    void updateObjects(float dt)
    {
        for (auto& obj : m_objects) {
            obj.update(dt);
        }
    }
};

class VerletSimulationManager : public SimulationBase{
private:
    Settings& settings = Settings::GetInstance();
    Solver solver;
    bool simulationRunning = false;
    Uint32 last_spawn_time = 0;

    static void DrawCircle(float x, float y, float radius, Color color)
    {
        const float DEG2RAD = 3.14159f / 180;
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(color.red, color.green, color.blue);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i++) {
            float degInRad = (float)i * DEG2RAD;
            glVertex2f(x + cos(degInRad) * radius, y + sin(degInRad) * radius);
        }
        glEnd();
    }

public:
    VerletSimulationManager() = default;
    ~VerletSimulationManager() = default;

    void Initialize() override
    {
        std::cout << "VerletSimulationManager::Initialize()" << std::endl;
        solver.addObject({(float)settings.GetWindow().width / 2.0f, (float)settings.GetWindow().height / 2.0f}, 50.0f);
    }

    void Run() override {
        if (!simulationRunning)
            return;

        // Solver configuration
        solver.setSimulationUpdateRate((uint32_t)settings.GetSimulations().fps);
        solver.setSubStepsCount((uint32_t)settings.GetSimulations().verlet.subSteps);

        // Set simulation attributes
        const float object_spawn_delay = settings.GetSimulations().verlet.object_spawn_delay;
        const float object_spawn_speed = settings.GetSimulations().verlet.object_spawn_speed;
        const float object_min_radius = settings.GetSimulations().verlet.object_min_radius;
        const float object_max_radius = settings.GetSimulations().verlet.object_max_radius;

        // Make a custom spawn location function
        const Vector2<float> object_spawn_position = {(float)settings.GetWindow().width / 2.0f, object_max_radius};
        const uint32_t max_objects_count = settings.GetSimulations().verlet.max_objects_count;
        const float max_angle = settings.GetSimulations().verlet.max_angle;

        // Spawn objects
        Uint32 current_time = SDL_GetTicks();
        float elapsed_time = (float)(current_time - last_spawn_time) / 1000.0f;

        if (elapsed_time >= object_spawn_delay && solver.getObjectsCount() < max_objects_count) {
            std::cout << "Spawn object" << std::endl;
            auto &object = solver.addObject(object_spawn_position,
                                            RNGf::getRange(object_min_radius, object_max_radius));
            const float t = solver.getTime();
            const float angle = max_angle * sin(t) + PI * 0.5f;
            solver.setObjectVelocity(object, object_spawn_speed * Vector2<float>{cos(angle), sin(angle)});
            object.color = getRainbow(t);

            last_spawn_time = current_time;
        }

        solver.update();

        // Rendering part
        for (const auto& object : solver.getObjects()) {
            DrawCircle(object.position.x, object.position.y, object.radius, object.color);
        }
    }

    void RenderUI() override {
        ImGui::SliderInt("Substeps", &settings.GetSimulations().verlet.subSteps, 1, 20);

        if (ImGui::Button("Start Simulation")) {
            simulationRunning = true;
        }

        ImGui::SameLine();

        if (ImGui::Button("Stop Simulation")) {
            simulationRunning = false;
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Object Settings")) {
            float object_spawn_delay = settings.GetSimulations().verlet.object_spawn_delay;
            if (ImGui::SliderFloat("Object Spawn Delay", &object_spawn_delay, 0.0f, 10.0f)) {
                settings.GetSimulations().verlet.object_spawn_delay = object_spawn_delay;
            }

            float object_spawn_speed = settings.GetSimulations().verlet.object_spawn_speed;
            if (ImGui::SliderFloat("Object Spawn Speed", &object_spawn_speed, 0.0f, 10.0f)) {
                settings.GetSimulations().verlet.object_spawn_speed = object_spawn_speed;
            }

            float object_min_radius = settings.GetSimulations().verlet.object_min_radius;
            if (ImGui::SliderFloat("Object Min Radius", &object_min_radius, 0.0f, 100.0f)) {
                settings.GetSimulations().verlet.object_min_radius = object_min_radius;
            }

            float object_max_radius = settings.GetSimulations().verlet.object_max_radius;
            if (ImGui::SliderFloat("Object Max Radius", &object_max_radius, 0.0f, 100.0f)) {
                settings.GetSimulations().verlet.object_max_radius = object_max_radius;
            }

            uint32_t max_objects_count = settings.GetSimulations().verlet.max_objects_count;
            if (ImGui::SliderInt("Max Objects Count", (int*)&max_objects_count, 1, 1000)) {
                settings.GetSimulations().verlet.max_objects_count = max_objects_count;
            }

            float max_angle = settings.GetSimulations().verlet.max_angle;
            if (ImGui::SliderFloat("Max Angle", &max_angle, 0.0f, 2*PI)) {
                settings.GetSimulations().verlet.max_angle = max_angle;
            }
        }

        if (ImGui::CollapsingHeader("Simulation Info")) {
            ImGui::BeginGroup();
            ImGui::Text("Number of objects: %llu", solver.getObjectsCount());
            ImGui::Text("Simulation time: %.3f", solver.getTime());
            ImGui::Text("Simulation update rate: %.3f", solver.getStepDt());
            ImGui::EndGroup();
        }
    }

    std::string GetName() override {
        return "Verlet Simulation";
    }

    static Color getRainbow(float t)
    {
        const float r = sin(t);
        const float g = sin(t + 0.33f * 2.0f * PI);
        const float b = sin(t + 0.66f * 2.0f * PI);
        return {r * r, g * g, b * b};
    }
};

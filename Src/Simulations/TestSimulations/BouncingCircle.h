#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <random>
#include <vector>

namespace BouncingCircleSimulation {
    class BouncingCircle {
    private:
        float x, y;
        float radius;
        float vx, vy;
        float mass;
        float windowWidth, windowHeight;
        float red, green, blue;
        float gravity;
        float restitution;

        void CheckCollisionWithWindow()
        {
            if (x - radius < 0) {
                vx = std::abs(vx) * restitution;  // scale down the velocity by the restitution
                x = radius;
            } else if (x + radius > windowWidth) {
                vx = -std::abs(vx) * restitution;  // scale down the velocity by the restitution
                x = windowWidth - radius;
            }
            if (y - radius < 0) {
                vy = std::abs(vy) * restitution;  // scale down the velocity by the restitution
                y = radius;
            } else if (y + radius > windowHeight) {
                vy = -std::abs(vy) * restitution;  // scale down the velocity by the restitution
                y = windowHeight - radius;
            }
        }

    public:
        BouncingCircle(float x, float y, float radius, float vx, float vy, float mass, float restitution, float gravity, float windowWidth, float windowHeight)
                : x(x), y(y), radius(radius), vx(vx), vy(vy), mass(mass), restitution(restitution), gravity(gravity), windowWidth(windowWidth), windowHeight(windowHeight)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(0.0f, 1.0f);
            red = dist(gen);
            green = dist(gen);
            blue = dist(gen);
        }

        [[nodiscard]] bool CheckCollisionWithCircle(const BouncingCircle& circle2) const
        {
            float dx = x - circle2.x;
            float dy = y - circle2.y;
            float distance = sqrt(dx * dx + dy * dy);

            return distance < radius + circle2.radius;
        }

        static void SolveCollision(BouncingCircle& circle1, BouncingCircle& circle2)
        {
            float dx = circle1.x - circle2.x;
            float dy = circle1.y - circle2.y;
            float distance = sqrt(dx * dx + dy * dy);

            float nx = dx / distance;
            float ny = dy / distance;

            // Compute overlap, assuming distance < r1 + r2
            float overlap = 0.5f * (distance - circle1.radius - circle2.radius);

            // Displace current circle
            circle1.x -= overlap * nx;
            circle1.y -= overlap * ny;

            // Displace other circle
            circle2.x += overlap * nx;
            circle2.y += overlap * ny;

            float tx = -ny;
            float ty = nx;

            float dpTan1 = circle1.vx * tx + circle1.vy * ty;
            float dpTan2 = circle2.vx * tx + circle2.vy * ty;

            float dpNorm1 = circle1.vx * nx + circle1.vy * ny;
            float dpNorm2 = circle2.vx * nx + circle2.vy * ny;

            float m1 = (dpNorm1 * (circle1.mass - circle2.mass) + 2.0f * circle2.mass * dpNorm2) / (circle1.mass + circle2.mass);
            float m2 = (dpNorm2 * (circle2.mass - circle1.mass) + 2.0f * circle1.mass * dpNorm1) / (circle1.mass + circle2.mass);

            circle1.vx = tx * dpTan1 + nx * m1;
            circle1.vy = ty * dpTan1 + ny * m1;
            circle2.vx = tx * dpTan2 + nx * m2;

            circle1.vx = (tx * dpTan1 + nx * m1) * circle1.restitution;  // scale down the velocity by the restitution
            circle1.vy = (ty * dpTan1 + ny * m1) * circle1.restitution;  // scale down the velocity by the restitution
            circle2.vx = (tx * dpTan2 + nx * m2) * circle2.restitution;  // scale down the velocity by the restitution
        }

        static void FindCollisions(std::vector<BouncingCircle>& circles)
        {
            for (auto it = circles.begin(); it != circles.end(); ++it) {
                for (auto jt = std::next(it); jt != circles.end(); ++jt) {
                    if (it->CheckCollisionWithCircle(*jt)) {
                        SolveCollision(*it, *jt);
                    }
                }
            }
        }

        // Update function to include effect of gravity on vertical velocity
        void Update(float dt, std::vector<BouncingCircle>& circles)
        {
            x += vx * dt;
            y += vy * dt;
            vy += gravity * dt;  // Apply gravitational acceleration to vertical velocity

            CheckCollisionWithWindow();
            FindCollisions(circles);
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

    struct BouncingCircleSettings {
        std::vector<BouncingCircle> bouncingCircles;
        Settings& settings = Settings::GetInstance();

        // New variables for controlling the spawn timing
        float spawnDelay = 0.5f; // Delay between each spawn in seconds. Modify as needed
        float lastSpawnTime = 0.0f;
        int numCirclesToSpawn = 1000; // Total number of circles you want to spawn

        // Function to spawn circles
        [[maybe_unused]] void spawnCircles(float currentTime) {
            if (bouncingCircles.size() >= numCirclesToSpawn) {
                return;
            }

            if (currentTime - lastSpawnTime >= spawnDelay) {
                CreateRandomBouncingCircle();
                lastSpawnTime = currentTime;
            }
        }

        // Adjusted to create a single circle
        void CreateRandomBouncingCircle() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> distRadius(25.0f, 50.0f);
            float radius = 15;
            float x = settings.GetWindow().width / 2.0f;  // Circle spawns at window center
            float y = settings.GetWindow().height / 2.0f;  // Circle spawns at window center
            std::uniform_real_distribution<float> distSpeedX(-5.0f, 5.0f);
            float speedX = distSpeedX(gen);
            float speedY = 0.0f;  // No initial vertical speed
            std::uniform_real_distribution<float> distMass(1.0f, 5.0f);
            float mass = distMass(gen);
            float gravity = 0.8f;  // Set gravity (acceleration due to gravity)
            float restitution = 0.95f;  // Set restitution (energy loss due to collision)

            bouncingCircles.emplace_back(x, y, radius, speedX, speedY, mass, restitution, gravity, settings.GetWindow().width, settings.GetWindow().height);
        }
    } bC_settings;

    void Initialize()
    {
    }

    void Update(float currentTime) { // Assuming you have a way to get the current time
        bC_settings.spawnCircles(currentTime);

        for (auto& circle : bC_settings.bouncingCircles) {
            circle.Update(bC_settings.settings.GetSimulations().dt, bC_settings.bouncingCircles);
        }
    }


    void Render()
    {
        for (const auto& circle : bC_settings.bouncingCircles) {
            circle.Render();
        }
    }
}

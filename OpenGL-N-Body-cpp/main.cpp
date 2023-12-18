#include <GLFW/glfw3.h>
#include <vector>
#include "Body.h"
#include <cstdlib> // For rand() and srand()
#include <ctime>
#include "Constants.h"
#include "Vector.h"
#include <iostream>

void run_simulation(std::vector<Body>& bodies, float timeStep) {
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = 0; j < bodies.size(); ++j) {
            if (i != j) {
                bodies[i].gravitate(bodies[j]);
            }
        }
    }

    for (Body& body : bodies) {
        body.updatePosition(timeStep);
    }
}


int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1600, 1200, "Gravity Simulation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwMakeContextCurrent(window);

    // Set up viewport and projection
    glViewport(0, 0, 1600, 1200);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1600.0, 1200.0, 0.0, -1.0, 1.0); // Set coordinate system
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize bodies
    std::vector<Body> bodies;

    // Create smaller orbiting bodies
    for (int i = 0; i < BODY_COUNT; ++i) {
        float distance = static_cast<float>(rand() % 300 + 150); // Example: 150 to 499 units away
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * PI; // Random angle

        // Initialize random position
        float x_pos = 800.0f + distance * cos(angle);
        float y_pos = 600.0f + distance * sin(angle);

        // Calculate velocity for a rough circular orbit
        float velocityMagnitude = sqrt(10000.0f * 600.0f / distance);
        float x_vel = -velocityMagnitude * sin(angle);
        float y_vel = velocityMagnitude * cos(angle);

        // Add the body
        bodies.emplace_back(0.5f, 4.0f, x_pos, y_pos, x_vel, y_vel, 0.0f, 0.0f, 255.0f, 0.0f, 255.0f);
    }

    // Create central massive body
    bodies.emplace_back(8000.0f, 30.0f, 800.0f, 600.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    double lastTime = glfwGetTime();
    int frameCount = 0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        run_simulation(bodies, 0.01f); // Update simulation

        glLoadIdentity(); // Reset transformations

        // Collision detection and resolution
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = i + 1; j < bodies.size(); ++j) {
                if (bodies[i].checkCollision(bodies[j])) {
                    bodies[i].resolveCollision(bodies[j]);
                }
            }
        }

        // Render bodies and other visual updates
        glLoadIdentity();
        for (Body& body : bodies) {
            body.updateColorBasedOnVelocity();
            body.render();
            body.renderTrail();
        }

        double currentTime = glfwGetTime();
        frameCount++;
        if (currentTime - lastTime >= 1.0) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            lastTime += 1.0;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
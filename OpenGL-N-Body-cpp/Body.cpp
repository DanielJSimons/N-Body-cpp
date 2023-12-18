#include "Body.h"
#include <GLFW/glfw3.h> 
#include "constants.h" 
#include "Vector.h"

Body::Body(float mass, float radius, float x_pos, float y_pos, float x_vel, float y_vel, float x_acc, float y_acc, 
    float red, float green, float blue)
    : mass(mass), radius(radius), x_pos(x_pos), y_pos(y_pos), x_vel(x_vel), y_vel(y_vel), x_acc(x_acc), y_acc(y_acc), speed(0.0f),
    red(red), green(green), blue(blue) {}

Body::~Body() {
    
}


bool Body::checkCollision(const Body& other) const {
    // Checking if bodies have collided
    float dx = x_pos - other.x_pos;
    float dy = y_pos - other.y_pos;
    float distSquared = dx * dx + dy * dy;
    float radiusSum = radius + other.radius;
    return distSquared < (radiusSum * radiusSum);
}


double deg2rad(double degrees) {
    return degrees * 4.0 * atan(1.0) / 180.0;
}

void Body::gravitate(Body& otherBody) {
    // Simple vector calculations
    float dx = otherBody.x_pos - x_pos;
    float dy = otherBody.y_pos - y_pos;
    float distSquared = dx * dx + dy * dy;

    if (distSquared == 0) return;

    float distance = sqrt(distSquared);
    float forceMagnitude = G * mass * otherBody.mass / distSquared;

    x_acc = forceMagnitude * dx / (distance * mass);
    y_acc = forceMagnitude * dy / (distance * mass);
}


void Body::move(float x_offset, float y_offset) {
    x_pos += x_offset;
    y_pos += y_offset;
}


void Body::render() {
    // Set the colour
    glColor3f(red, green, blue);

    // Draw the body as a circle
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float degInRad = deg2rad(i);
        glVertex2f(cos(degInRad) * radius + x_pos, sin(degInRad) * radius + y_pos);
    }
    glEnd();
}


void Body::renderTrail() const {

    glColor3f(100.0f, 10.00f, 100.0f);

    glBegin(GL_LINES);
    for (const auto& pos : trail) {
        glVertex2f(pos.first, pos.second);
    }
    glEnd();
}


void Body::resolveCollision(Body& other) {
    // Temporary variables to hold initial velocities
    float v1x_initial = x_vel;
    float v1y_initial = y_vel;
    float v2x_initial = other.x_vel;
    float v2y_initial = other.y_vel;

    // Calculating final velocities in x-direction
    x_vel = ((mass - other.mass) * v1x_initial + 2 * other.mass * v2x_initial) / (mass + other.mass);
    other.x_vel = ((other.mass - mass) * v2x_initial + 2 * mass * v1x_initial) / (mass + other.mass);

    // Calculating final velocities in y-direction
    y_vel = ((mass - other.mass) * v1y_initial + 2 * other.mass * v2y_initial) / (mass + other.mass);
    other.y_vel = ((other.mass - mass) * v2y_initial + 2 * mass * v1y_initial) / (mass + other.mass);
}


void Body::updateColorBasedOnVelocity() {
    float speed = sqrt(x_vel * x_vel + y_vel * y_vel);
    float maxSpeed = 220.0f; // Maximum speed for red color
    float minSpeedForColorChange = 120.0f; // Minimum speed for color change (white to orange)
    float midSpeed = (maxSpeed + minSpeedForColorChange) / 2.0f; // Midpoint for orange to red transition

    // Normalize the speed to a range between 0 and 1
    float normalizedSpeed = std::min(speed / maxSpeed, 1.0f);

    if (speed < minSpeedForColorChange) {
        // Below min speed - color is white
        red = 1.0f;
        green = 1.0f;
        blue = 1.0f;
    }
    else if (speed < midSpeed) {
        // Between min speed and mid speed - transition from white to orange
        float transition = (speed - minSpeedForColorChange) / (midSpeed - minSpeedForColorChange);
        red = 1.0f;
        green = 1.0f - (0.35f * transition); // Transition green from 1 to 0.65
        blue = 1.0f - transition; // Transition blue from 1 to 0
    }
    else {
        // Above mid speed - transition from orange to red
        float transition = (speed - midSpeed) / (maxSpeed - midSpeed);
        red = 1.0f;
        green = 0.65f * (1.0f - transition); // Transition green from 0.65 to 0
        blue = 0.0f; // Blue stays 0
    }
}


void Body::updatePosition(float timeStep) {
    x_vel += x_acc * timeStep;
    y_vel += y_acc * timeStep;

    x_pos += x_vel * timeStep;
    y_pos += y_vel * timeStep;

    updateTrail();
}


void Body::updateTrail() {

    ++updateCount;

    speed = sqrt(x_vel * x_vel + y_vel * y_vel);
    static const float maxSpeed = 200.0f;

    float normalizedSpeed = std::min(speed / maxSpeed, 1.0f);

    size_t speedBasedTrailLength = static_cast<size_t>((1.0f - normalizedSpeed) * trailLength);

    size_t currentMaxTrailLength = std::min(updateCount, speedBasedTrailLength);

    trail.emplace_front(x_pos, y_pos);

    while (trail.size() > currentMaxTrailLength) {
        trail.pop_back();
    }
}
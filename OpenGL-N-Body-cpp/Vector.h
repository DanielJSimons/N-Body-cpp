#pragma once
#include <cmath>

struct Vector {
    float x, y;

    Vector(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    float magnitudeSquared() const {
        return x * x + y * y;
    }

    Vector normalized() const {
        float mag = sqrt(magnitudeSquared());
        return Vector(x / mag, y / mag);
    }

    Vector operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y);
    }

    Vector& operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector operator*(float scalar) const {
        return Vector(x * scalar, y * scalar);
    }
};
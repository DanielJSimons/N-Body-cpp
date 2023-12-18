#ifndef BODY_H
#define BODY_H
#include "Constants.h"
#include <deque>

class Body {
public:

    Body(float mass, float radius, float x_pos, float y_pos, float x_vel, float y_vel, float x_acc, float y_acc,
        float red, float green, float blue);

    ~Body();

    void move(float x_offset, float y_offset);
    void gravitate(Body& otherBody);
    void updatePosition(float timeStep);
    void updateTrail();
    void renderTrail() const;
    void render(); 
    bool checkCollision(const Body& other) const;
    void resolveCollision(Body& other);
    void updateColorBasedOnVelocity();

    void updateSpeed() {
        speed = sqrt(x_vel * x_vel + y_vel * y_vel);
    }


private:

    float mass;
    float radius;
    float x_pos;
    float y_pos;
    float x_vel;
    float y_vel;
    float x_acc;
    float y_acc;

    float red, green, blue; 
    float speed;


    std::deque<std::pair<float, float>> trail;

    size_t updateCount = 0;

    static const size_t trailLength = MAX_TRACK_POINTS;
    static constexpr float scalingFactor = 0.08f;
};

#endif 

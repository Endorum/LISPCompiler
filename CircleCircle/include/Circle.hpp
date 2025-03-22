#pragma once

#include <cmath>

//#include "vmath.hpp"

typedef struct float2 {
    float x;
    float y;
}float2;

class Circle {
public:
    float2 position{};
    float2 velocity{};

    float radius=1;
    float mass=1;

    Circle(float2 pos, float r=1, float m=1)
        : position(pos), radius(r), mass(m) {}
};

inline bool CircleCircleCheck(Circle& c1, Circle& c2) {
    // Compute the distance squared between the centers
    float dx = c1.position.x - c2.position.x;
    float dy = c1.position.y - c2.position.y;
    float distanceSquared = dx * dx + dy * dy;

    // Compute the sum of the radii
    float radiusSum = c1.radius + c2.radius;

    // Check if the distance squared is less than or equal to the square of the radius sum
    return distanceSquared <= radiusSum * radiusSum;
}

// with m1 != m2
inline void CircleCircleSolver(Circle& c1, Circle& c2) {
    float x1 = c1.position.x;
    float y1 = c1.position.y;
    float v1x = c1.velocity.x;
    float v1y = c1.velocity.y;
    float m1 = c1.mass;

    float x2 = c2.position.x;
    float y2 = c2.position.y;
    float v2x = c2.velocity.x;
    float v2y = c2.velocity.y;
    float m2 = c2.mass;

    float phi = std::__math::atan((y2-y1)/(x2-x1));
    float sinphi = sin(phi);
    float cosphi = cos(phi);

    const float tmp1 = ( (m1-m2)*(v1x*cosphi + v1y*sinphi) + 2*m2*(v2x*cosphi + v2y*sinphi) );
    const float tmp2 = ( (m2-m1)*(v2x*cosphi + v2y*sinphi) + 2*m1*(v1x*cosphi + v1y*sinphi) );
    const float m1m2 = m1+m2;


    float u1x = (v1x*sinphi-v1y*cosphi)*sinphi + (tmp1 * cosphi) / m1m2;
    float u1y = (-v1x*sinphi+v1y*cosphi)*cosphi + (tmp1 * sinphi) / m1m2;

    float u2x = (v2x*sinphi-v2y*cosphi)*sinphi + (tmp2 * cosphi) / m1m2;
    float u2y = (-v2x*sinphi+v2y*cosphi)*cosphi + (tmp2 * sinphi) / m1m2;


    c1.velocity.x = u1x;
    c1.velocity.y = u1y;

    c2.velocity.x = u2x;
    c2.velocity.y = u2y;
}

// m1 == m2
inline void CircleCirlceEqual(Circle& c1, Circle& c2) {
    float x1 = c1.position.x;
    float y1 = c1.position.y;
    float v1x = c1.velocity.x;
    float v1y = c1.velocity.y;

    float x2 = c2.position.x;
    float y2 = c2.position.y;
    float v2x = c2.velocity.x;
    float v2y = c2.velocity.y;

    const float phi = std::__math::atan((y2-y1)/(x2-x1));
    float sinphi = sin(phi);
    float cosphi = cos(phi);

    float u1x = ( v1x*sinphi-v1y*cosphi)*sinphi + (v2x*cosphi+v2y*sinphi)*cosphi;
    float u1y = (-v1x*sinphi+v1y*cosphi)*cosphi + (v2x*cosphi+v2y*sinphi)*sinphi;
    float u2x = ( v2x*sinphi-v2y*cosphi)*sinphi + (v1x*cosphi+v1y*sinphi)*cosphi;
    float u2y = (-v2x*sinphi+v2y*cosphi)*cosphi + (v1x*cosphi+v1y*sinphi)*sinphi;

    c1.velocity.x = u1x;
    c1.velocity.y = u1y;

    c2.velocity.x = u2x;
    c2.velocity.y = u2y;
}
#pragma once

#include "vec3.h"

// A ray is a function P(t) = A = tb, where A is the ray's origin,
// b is the direction, and t is some unit from the origin.
class ray {
   public:
    ray() {}
    // Initialize the origin point and path direction
    // The parenthesized init is direct and reduces the number of
    // statement for initialization.
    ray(const point3& A, const vec3& b) : orig(A), dir(b) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    point3 at(double t) const {
        return orig + t * dir;
    }

   public:
    point3 orig;
    vec3 dir;
};

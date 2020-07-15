#pragma once

#include <cmath>
#include <iostream>

#include "common.h"

using std::sqrt;

class vec3 {
   public:
    // The part after the ":" is a list initializer.
    // These member variables will be initialized before
    // the body of the constructor executes.
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    // The const after the method declaration creates a
    // const-qualified "this". Meaning the member variables
    // cannot be altered.
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }
    // Negation operator
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    // Getter. Const so no alterations can be done.
    double operator[](int i) const { return e[i]; }
    // Setter. Returns a reference.
    double& operator[](int i) { return e[i]; }

    // Vector addition.
    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }
    vec3& operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

   public:
    double e[3];
};

// Aliases for point and colour
using point3 = vec3;  // 3D point
using colour = vec3;  // RGB

// Utilities
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Vector addition and subrtaction
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Vector multiplication and scalr multiplication
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// Returns a random vec in within the unit sphere
vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}

vec3 random_unit_vector() {
    // Random angle on sphere
    auto a = random_double(0, 2 * pi);
    // Random value in range
    auto z = random_double(-1, 1);
    auto r = sqrt(1 - z * z);
    // Return the new unit vector
    return vec3(r * cos(a), r * sin(a), z);
}

// For the focus distance
vec3 random_in_unit_disk() {
    while (true) {
        vec3 p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}

vec3 reflect(const vec3& v, const vec3& n) {
    //v + 2b
    return v - 2 * dot(v, n) * n;
}

// Snell's law implementation
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = dot(-uv, n);
    // Distance from normal
    vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    // Distance from surface
    vec3 r_out_perp = -sqrt(1 - r_out_parallel.length_squared()) * n;
    return r_out_parallel + r_out_perp;
}
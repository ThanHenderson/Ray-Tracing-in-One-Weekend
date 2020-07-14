#pragma once

#include "ray.h"
// This is so the materails will determine the how the rays interact with a surface
class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    // For point p(t) on the ray
    double t;
    bool front_face;

    // Sets the face normal depending on which side of the surface is hit
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
   public:
    // Virtual functions can be overridden in a derived class. Setting it
    // to zero means that you MUST derive a class and implement the function.
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

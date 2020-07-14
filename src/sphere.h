#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
   public:
    sphere(){};
    sphere(point3 cen, double r, shared_ptr<material> m) : centre(cen), radius(r), mat_ptr(m){};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;

   public:
    point3 centre;
    double radius;
    shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - centre;
    // A vector dotted with itself is the squared length of the vector
    auto a = dot(r.direction(), r.direction());
    // Use half b since the factors of 2 cancel in the quad-eq
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    // If the discriminant is negative then there are no roots
    // i.e. no contact with the sphere
    auto discriminant = half_b * half_b - a * c;
    // If a hittable is hit
    if (discriminant > 0) {
        auto root = sqrt(discriminant);
        auto temp = (-half_b - root) / a;
        // Setting a record of the hit locations at t
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - centre) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - centre) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

/**I discovered the studio in 2019 through the Rust game engine Amethyst. My interest in Embark initially stemmed from the idea of using Rust as the programming language of choice. After looking deeper, Embark seemed to be a place with an interesting idea and approach that is filled with knowledgeable and determined industry professionals. 

Being driven by curiosity, I'm excited to see where the studio will grow to.


I am particularly interested in graphics and rendering. I'm looking into master's programs for computer graphics planning to focus on light transport simulation and global illumination.

I am looking for an internship beginning in January 2021. But will also be interested in internships and other work placements during my masters degree.

I have no industry experience in computer games programming or graphics programming but I have done a few personal projects in the area.
**/
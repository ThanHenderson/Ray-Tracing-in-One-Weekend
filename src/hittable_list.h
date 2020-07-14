#pragma once

#include <memory>
#include <vector>

#include "hittable.h"

using std::make_shared;
// Returns a smart pointer that retains shared ownershp of an object
using std::shared_ptr;

class hittable_list : public hittable {
   public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }
    void clear() { objects.clear(); }
    // Add a value to the end of the vector (obj array)
    void add(shared_ptr<hittable> object) { objects.push_back(object); }
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

   public:
    // Vector to store the hittable objects
    std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = tmax;

    for (const auto& object : objects) {
        // -> references a pointer to a structure
        if (object->hit(r, tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}
#pragma once

#include "common.h"

class camera {
   public:
    // Allows for custom fov and ratio
    camera(
        double vfov,  // Vertical degree angle
        double aspect_ratio,
        point3 look_from,
        point3 look_at,
        vec3 vup,  // The 'view up' direction. Allows to specift rotation from axis
        double aperture,
        double focus_dist

    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 10.0;

        // Form an orthanormal basis for the cameras position
        w = unit_vector(look_from - look_at);  // Our camera points -w
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = look_from;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        lens_radius = aperture / 2;
    }

    ray get_ray(double a, double b) const {
        // Random x and y coords in the unit disk
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + a * horizontal + b * vertical - origin - offset);
    }

   private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
};
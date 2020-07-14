#pragma once

#include <iostream>

#include "vec3.h"

void write_colour(std::ostream& out, colour pixel_colour, int samples_per_pixel) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    // Divide by the total number of samples
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated value between 0 256 inclusive
    out << static_cast<int>(256 * clamp(r, 0.0, 0.9999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.9999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.9999)) << '\n';
}

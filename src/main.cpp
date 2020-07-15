#include <iostream>

#include "camera.h"
#include "colour.h"
#include "common.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

/** 
 * P3 format
 * 
 * P3
 * 2 2    //ROW, COL
 * 255    //Max colour
 * 255    0   0     0   255   0     //rgb triplets
 * 255  255   0     255 255 255
 * 
**/

double hit_sphere(const point3& centre, double radius, const ray& r) {
    vec3 oc = r.origin() - centre;
    // A vector dotted with itself is the squared length of the vector
    auto a = dot(r.direction(), r.direction());
    // Use half b since the factors of 2 cancel in the quad-eq
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    // If the discriminant is negative then there are no roots
    // i.e. no contact with the sphere
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return -1;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

colour ray_colour(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0)
        return colour(0, 0, 0);
    // Prevents inaccurate hits at t. Ie fixing shadow acne
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        colour attenuation;

        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_colour(scattered, world, depth - 1);

        return colour(0, 0, 0);
    }
    // Create a unit vector of the ray direction
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    // First colour is white and second is blue. LERP.
    return (1.0 - t) * colour(1.0, 1.0, 1.0) + t * colour(0.5, 0.7, 1.0);
}

hittable_list snowman_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(colour(0.6, 0.1, 0.1));
    world.add(make_shared<sphere>(point3(0, -1000.0, 0), 1000, ground_material));

    //  Bottom
    auto material1 = make_shared<metal>(colour(0.5, 0.5, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    // Middle
    auto material2 = make_shared<metal>(colour(0.5, 0.5, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(0, 2.3, 0), 0.7, material2));

    // Top
    auto material3 = make_shared<metal>(colour(0.5, 0.5, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(0, 3.2, 0), 0.4, material3));

    // Distant Spheres
    for (int q = -10; q < 10; q++) {
        auto albedo = colour::random(0.5, 1);
        auto fuzz = random_double(0.0, 0.5);
        auto material = make_shared<metal>(albedo, fuzz);
        world.add(make_shared<sphere>(point3(q, 0.2, 10.0 * random_double()), 0.2, material));
    }

    return world;
}

hittable_list simple_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(colour(0.9, 0.1, 0.1));
    world.add(make_shared<sphere>(point3(0, -1000.0, 0), 1000, ground_material));

    for (int m = -5; m < 5; m++) {
        for (int n = 1; n < 3; n++) {
            auto material_roulette = random_double();
            point3 centre(m + 0.9 * random_double(), 0.2, n + 0.9 * random_double());

            if ((centre - point3(2, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> material;

                if (material_roulette < 0.8) {
                    auto albedo = colour::random(0.5, 1);
                    auto fuzz = random_double(0.0, 0.5);
                    material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(centre, 0.2, material));
                } else {
                    material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(centre, 0.2, material));
                }
            }
        }
    }

    auto material1 = make_shared<metal>(colour(1.0, 0.75, 0.8), 0.0);
    world.add(make_shared<sphere>(point3(-1, 1, 0), 1.0, material1));

    auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(1, 1, 0), 1.0, material3));

    return world;
}

hittable_list generate_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(colour(0.2, 0.3, 0.2));
    world.add(make_shared<sphere>(point3(0, -1000.0, 0), 1000, ground_material));

    for (int m = -11; m < 11; m++) {
        for (int n = -11; n < 11; n++) {
            auto material_roulette = random_double();
            point3 centre(m + 0.9 * random_double(), 0.2, n + 0.9 * random_double());

            if ((centre - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> material;

                if (material_roulette < 0.8) {
                    // Matte, easier to render
                    auto albedo = colour::random() * colour::random();
                    material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(centre, 0.2, material));
                } else if (material_roulette < 0.95) {
                    // Metal
                    auto albedo = colour::random(0.5, 1);
                    auto fuzz = random_double(0.0, 0.5);
                    material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(centre, 0.2, material));
                } else {
                    // Glass
                    material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(centre, 0.2, material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main() {
    const auto aspect_ratio = 9.0 / 16.0;
    const int image_width = 768;
    // w/w/h = h
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // Complex Scene
    // auto world = generate_scene();

    // point3 lookfrom(13, 2, 3);
    // point3 lookat(0, 0, 0);
    // vec3 vup(0, 1, 0);
    // auto dist_to_focus = 10.0;
    // auto aperture = 0.1;

    // Simple scene
    // auto world = simple_scene();

    // point3 lookfrom(0, 2, 10);
    // point3 lookat(0, 0, 0);
    // vec3 vup(0, 1, 0);
    // auto dist_to_focus = 10.0;
    // auto aperture = 0.05;

    // Snowman scene
    auto world = snowman_scene();

    point3 lookfrom(0, 2, 10);
    point3 lookat(0, 2, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.05;

    camera cam(30, aspect_ratio, lookfrom, lookat, vup, aperture, dist_to_focus);

    std::cout
        << "P3\n"
        << image_width << ' ' << image_height << "\n255\n";

    for (int i = image_height - 1; i >= 0; i--) {
        std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
        for (int j = 0; j < image_width; j++) {
            colour pixel_colour(0, 0, 0);
            // Loop for antialiasing
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = double(j + random_double()) / (image_width - 1);
                auto v = double(i + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                // Update the rgb value of each pixel in the image. Reflecting
                // from the surface.
                pixel_colour += ray_colour(r, world, max_depth);
            }
            // Pixels are written out in rows from left to right. With sampling
            write_colour(std::cout, pixel_colour, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}
#pragma once

/**
The materials need to:
1. Produce a scattered ray, or absorb the ray completely
2. If scattered, say how much the ray should be scattered
**/

class material {
   public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const = 0;
};

// Utility Functions
double schlick(double cosine, double ref_idx) {
    //Schlick's poly nomial approximation for glass reflectivity
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

// MATERIALS
// Matte material
class lambertian : public material {
   public:
    lambertian(const colour& a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction);
        // Attenuation is the reduction of intensity of the light
        attenuation = albedo;
        return true;
    }

   public:
    colour albedo;
};

// Metal material
class metal : public material {
   public:
    // Fuzz is a pararmeter the controls the amount of fuzziness of the reflections
    metal(const colour& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        // Added the fuzz*random in unit sphere to the reflected ray to fuzzify the reflection
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

   public:
    colour albedo;
    double fuzz;
};

// Dielectric material
// Use negative radius when creating a sphere to make a hollow object;
// Doing such reverses the surface normal from outward to inward.
class dielectric : public material {
   public:
    dielectric(double ri) : ref_idx(ri) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const {
        attenuation = colour(1.0, 1.0, 1.0);
        double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : ref_idx;
        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0) {
            // Reflect in relation to the in direction
            vec3 reflected = reflect(unit_direction, rec.normal);
            // Scatter from point to reflected
            scattered = ray(rec.p, reflected);
            return true;
        }

        // Schlick's approx
        double reflect_prob = schlick(cos_theta, etai_over_etat);

        if (random_double() < reflect_prob) {
            // Reflect in relation to the in direction
            vec3 reflected = reflect(unit_direction, rec.normal);
            // Scatter from point to reflected
            scattered = ray(rec.p, reflected);
            return true;
        }

        // Otherwise refract
        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = ray(rec.p, refracted);
        return true;
    }

   public:
    double ref_idx;
};

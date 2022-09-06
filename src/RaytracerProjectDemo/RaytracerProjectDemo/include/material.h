#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "utilities.h"

struct hit_record;

class material {
public:
	virtual ~material() = default;
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material{
    public:
        color albedo;

        explicit lambertian(const color & a) : albedo(a) {}

        bool scatter(const ray & r_in, const hit_record & rec, color & attenuation, ray & scattered) const override
		{
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
            {
				scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

class metal : public material {
	public:
		color albedo;
		float fuzz;

		explicit metal(const color& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {} // fuzziness is capped at 1

		bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
		{
			vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
			scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
		}
};

class dielectric: public material
{
private:
	static float reflectance(float cosine, float ref_index)
	{
		// Use Schlick's approximation for reflectance
		auto r0 = (1 - ref_index) / (1 + ref_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5.0f);
	}
public:
	float index_of_refraction; // index of refraction

	dielectric(float ior) : index_of_refraction(ior) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = color(1.0f, 1.0f, 1.0f);
		float refraction_ratio = rec.is_front_face ? (1.0f / index_of_refraction) : index_of_refraction;

		vec3 unit_dir = unit_vector(r_in.direction());
		float cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0f); // cos_theta does not go beyond 1.0
		float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

		bool can_refract = refraction_ratio * sin_theta < 1.0f; // sin_theta does not go beyond 1.0 as well
		vec3 direction;

		if (!can_refract)
		{
			direction = reflect(unit_dir, rec.normal);
		}
		else
		{
			direction = refract(unit_dir, rec.normal, refraction_ratio);
		}

		scattered = ray(rec.p, direction);
		return true; // always refracts
	}
};

#endif
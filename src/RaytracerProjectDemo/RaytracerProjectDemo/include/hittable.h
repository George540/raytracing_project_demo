#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "utilities.h"

class material;

struct hit_record
{
	point3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	float t{};
	bool is_front_face{};

	void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		is_front_face = dot(r.direction(), outward_normal) < 0.0f; // if > 0, ray is inside the sphere, else, otherwise
		normal = is_front_face ? outward_normal : -outward_normal;
	}
};

class hittable
{
public:
	virtual ~hittable() = default;
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif


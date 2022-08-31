#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
	point3 center;
	float radius;

	sphere() : center{ 0.0f, 0.0f, 0.0f }, radius(1.0f) {}
	sphere(point3 cen, float r) : center(cen), radius(r) {}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	// Using the quadratic formula to find the two points in the sphere
	// Notice how b has a factor two in it, so b = 2h, h = b/2 or half_b
	vec3 oc = r.origin() - center; // (P - C)
	auto a = r.direction().length_squared(); // r^2
	auto half_b = dot(oc, r.direction()); // 2tb
	auto c = oc.length_squared() - radius * radius; // (P - C)^2 - r^2

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;

	// Find the nearest root that lies in the acceptable range
	auto sqrtd = sqrt(discriminant);
	// Negative root is the first point of selection (might be the only point of intersection)
	auto root = (- half_b - sqrtd) / a;
	// If root is out of range min and max, pick positive (since there might be another point behind the object)
	if (root < t_min || t_max < root)
	{
		root = (-half_b + sqrtd) / a;
		// if it's still out of range, then there are no other collision points
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	//rec.normal = (rec.p - center) / radius;
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);

	return true;
}

#endif
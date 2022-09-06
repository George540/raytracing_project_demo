#ifndef CAMERA_H
#define CAMERA_H

#include "utilities.h"

class camera
{
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
public:
	camera(point3 lookfrom, point3 lookat, vec3 vup, float vfov, float aspect_ratio) // vfov: vertical field-of-view in degrees
	{
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2.0f);
		auto viewport_height = 2.0f * h;
		auto viewport_width = aspect_ratio * viewport_height;

		auto w = unit_vector(lookfrom - lookat);
		auto u = unit_vector(cross(vup, w));
		auto v = cross(w, u);

		origin = lookfrom;
		horizontal = viewport_width * u;
		vertical = viewport_height * v;
		lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;
	}

	ray get_ray(float s, float t) const
	{
		return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
	}
};

#endif
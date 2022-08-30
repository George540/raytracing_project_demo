#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
	// Variables
	point3 orig;
	vec3 dir;

	// Constructors
	ray() = default;
	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	// Accessors
	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }

	// Utility Functions
	/** 
	 * \brief the Linear Interpolation of a ray: P(t) = A + tb
	 * where P(t) = 3D position along a line in 3D,
	 * A = ray's origin(orig)
	 * b = ray's direction(dir)
	 * \param t the real number that moves point along the ray
	 * \return point in 3D space, P(t)
	 */
	point3 at(const float t) const
	{
		return orig + t * dir;
	}
};

#endif


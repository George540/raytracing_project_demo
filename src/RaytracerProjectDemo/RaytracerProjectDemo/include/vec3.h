#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3
{
	public:
		// Constructors & Destructors
		vec3() : coords{0 ,0 ,0} {}
		vec3(const float e0, const float e1, const float e2) : coords{e0, e1, e2} {}
		vec3(const vec3& v) : coords{v.coords[0], v.coords[1], v.coords[2]} {}
		~vec3() = default;

		// Coordinate Accessors
		float x() const { return coords[0]; }
		float y() const { return coords[1]; }
		float z() const { return coords[2]; }

		// Operators
		vec3& operator+=(const vec3& vector)
		{
			coords[0] += vector.coords[0];
			coords[1] += vector.coords[1];
			coords[2] += vector.coords[2];
			return *this;
		}

		vec3& operator*=(const float value)
		{
			coords[0] *= value;
			coords[1] *= value;
			coords[2] *= value;
			return *this;
		}

		vec3& operator/=(const float value)
		{
			return *this *= 1 / value;
		}

		// Length Functions
		float length() const // L
		{
			return sqrt(length_squared());
		}

		float length_squared() const // L^2
		{
			return coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2];
		}
		
	public:
		float coords[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

#endif
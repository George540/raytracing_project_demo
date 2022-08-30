#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3
{
	public:
		// Constructors
		vec3() : coords{0 ,0 ,0} {}
		vec3(const float e0, const float e1, const float e2) : coords{e0, e1, e2} {}
		//vec3(const vec3& v) : coords{v.coords[0], v.coords[1], v.coords[2]} {}
		//~vec3() = default;

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

		float coords[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

// Utility/Friend Functions
inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.coords[0] << ' ' << v.coords[1] << ' ' << v.coords[2];
}

inline vec3 operator+(const vec3& v, const vec3& u)
{
	return vec3(v.coords[0] + u.coords[0], v.coords[1] + u.coords[1], v.coords[2] + u.coords[2]);
}

inline vec3 operator-(const vec3& v, const vec3& u)
{
	return vec3(v.coords[0] - u.coords[0], v.coords[1] - u.coords[1], v.coords[2] - u.coords[2]);
}

inline vec3 operator*(const vec3& v, const vec3& u)
{
	return vec3(v.coords[0] * u.coords[0], v.coords[1] * u.coords[1], v.coords[2] * u.coords[2]);
}

inline vec3 operator*(float t, const vec3& v)
{
	return vec3(t * v.coords[0], t * v.coords[1], t * v.coords[2]);
}

inline vec3 operator*(const vec3& v, float t)
{
	return t * v;
}

inline vec3 operator/(vec3 v, float t)
{
	return (1 / t) * v;
}

inline float dot(const vec3& v, const vec3& u)
{
	return v.coords[0] * u.coords[0]
		+ v.coords[1] * u.coords[1]
		+ v.coords[2] * u.coords[2];
}

inline vec3 cross(const vec3& v, const vec3& u)
{
	return vec3(v.coords[1] * u.coords[2] - v.coords[2] * u.coords[1],
		v.coords[2] * u.coords[0] - v.coords[0] * u.coords[2],
		v.coords[0] * u.coords[1] - v.coords[1] * u.coords[0]);
}

inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}

#endif
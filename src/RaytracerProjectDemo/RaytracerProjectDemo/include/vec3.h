#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3
{
	public:
		// Variables
		float coords[3];

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

		static vec3 random()
		{
			return vec3(random_float(), random_float(), random_float());
		}

		static vec3 random(float min, float max)
		{
			return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
		}

		bool near_zero() const
		{
			// Return true if the vector is close to zero in all dimensions.
			const auto s = 1e-8;
			return (fabs(coords[0]) < s) && (fabs(coords[1]) < s) && (fabs(coords[2]) < s);
		}
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

inline vec3 operator-(const vec3&v)
{
	return vec3(-v.coords[0], -v.coords[1], -v.coords[2]);
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

// More vector calculations
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

// Random hits in objects
inline vec3 random_in_unit_sphere()
{
	while (true)
	{
		auto p = vec3::random(-1.0f, 1.0f);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

inline vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0f) // In the same hemisphere as the normal
	{
		return in_unit_sphere;
	}
	return -in_unit_sphere;
}

inline vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_unit_disk() // generate random point inside unit disks
{
	while (true)
	{
		auto p = vec3(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f), 0.0f);
		if (p.length_squared() >= 1.0f) continue;
		return p;
	}
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2.0f * dot(v, n) * n;
}

inline vec3 refract(const vec3& vu, const vec3& n, float etai_over_etat)
{
	auto cos_theta = fmin(dot(-vu, n), 1.0f);
	vec3 r_out_perp = etai_over_etat * (vu + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

#endif
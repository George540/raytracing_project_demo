// RaytracerProjectDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "vec3.h"
#include "color.h"
#include "ray.h"

#include <iostream>

float hit_sphere(const point3& center, const float radius, const ray& r)
{
    // Using the quadratic formula to find the two points in the sphere
    // Notice how b has a factor two in it, so b = 2h, h = b/2 or half_b
    vec3 oc = r.origin() - center; // (P - C)
    auto a = r.direction().length_squared(); // r^2
    auto half_b = dot(oc, r.direction()); // 2tb
    auto c = dot(oc, oc) - radius * radius; // (P - C)^2 - r^2
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return -1.0f;
    }
    return (-half_b - sqrt(discriminant)) / a;
}

/**
 * \brief Linearly blends white and blue depending on the height of the y
 * coordinate after scaling the ray direction to unit length
 * Formula: blendedValue=(1−t)⋅startValue+t⋅endValue
 * \param r ray to color
 * \return colored version of the ray
 */
color ray_color(const ray& r)
{
    auto t = hit_sphere(point3(0.0f, 0.0f, -1.0f), 0.5f, r);
    if (t > 0.0f)
    {
        vec3 N = unit_vector(r.at(t) - vec3(0.0f, 0.0f, -1.0f));
        return 0.5f * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }
    auto unity_direction = unit_vector(r.direction());
    t = 0.5f * (unity_direction.y() + 1.0f);
    return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0f / 9.0f;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0f;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0f;

    auto origin = point3(0.0f, 0.0f, 0.0f);
    auto horizontal = vec3(viewport_width, 0.0f, 0.0f);
    auto vertical = vec3(0.0f, viewport_height, 0.0f);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0.0f, 0.0f, focal_length);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScan lines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto v = float(i) / (image_width - 1);
            auto u = float(j) / (image_height - 1);
            ray r(origin, lower_left_corner + v * horizontal + u * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

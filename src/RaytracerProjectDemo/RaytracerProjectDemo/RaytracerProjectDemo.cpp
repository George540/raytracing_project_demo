// RaytracerProjectDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "utilities.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth)
{
	hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
    {
        return color(0.0f, 0.0f, 0.0f);
    }

    // if t_min is absolute 0, it creates acne
	if (world.hit(r, 0.001f, infinity, rec)) {
		//return 0.5f * (rec.normal + color(1.0f, 1.0f, 1.0f)); // Old normal coloration
        point3 target = rec.p + rec.normal + random_unit_vector();
        return 0.5f * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
	}
    // if hit nothing, render gradient bg
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0f / 9.0f;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 200;
    const int max_depth = 50;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f));
    world.add(make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f));

    // Camera
    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto v = (static_cast<float>(i) + random_float()) / (image_width - 1);
                auto u = (static_cast<float>(j) + random_float()) / (image_height - 1);
                ray r = cam.get_ray(v, u);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
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

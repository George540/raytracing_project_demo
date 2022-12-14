// RaytracerProjectDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "utilities.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

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
        ray scattered;
        color attenuation;
        if (rec.mat_ptr-> scatter(r, rec, attenuation, scattered))
        {
	        return attenuation * ray_color(scattered, world, depth - 1);
        }
	}
    // if hit nothing, render gradient bg
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5f, 0.5f, 0.5f));
    world.add(make_shared<sphere>(point3(0.0f, -1000.0f, 0.0f), 1000.0f, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
	        const auto choose_mat = random_float();
            point3 center(static_cast<float>(a) + 0.9f * random_float(), 0.2f, static_cast<float>(b) + 0.9f * random_float());

            if ((center - point3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8f) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2f, sphere_material));
                }
                else if (choose_mat < 0.95f) {
                    // metal
                    auto albedo = color::random(0.5f, 1.0f);
                    auto fuzz = random_float(0.0f, 0.5f);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2f, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5f);
                    world.add(make_shared<sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5f);
    world.add(make_shared<sphere>(point3(0.0f, 1.0f, 0.0f), 1.0f, material1));

    auto material2 = make_shared<lambertian>(color(0.4f, 0.2f, 0.1f));
    world.add(make_shared<sphere>(point3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

    auto material3 = make_shared<metal>(color(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(make_shared<sphere>(point3(4.0f, 1.0f, 0.0f), 1.0f, material3));

    return world;
}

int main() {

    // Image
    const auto aspect_ratio = 16.0f / 9.0f;
    const int image_width = 500;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 200;
    const int max_depth = 50;

    // World
    auto world = random_scene();

    // Camera
    point3 lookfrom(13.0f, 2.0f, 3.0f);
    point3 lookat(0.0f, 0.0f, 0.0f);
    vec3 vup(0.0f, 1.0f, 0.0f);
    auto dist_to_focus = 10.0f;
    auto aperture = 0.1f;

    camera cam(lookfrom, lookat, vup, 20.0f, aspect_ratio, aperture, dist_to_focus);

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

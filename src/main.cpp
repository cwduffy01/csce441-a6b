#include <iostream>
#include <string>
#include <limits>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Image.h"
#include "Camera.h"
#include "Material.h"
#include "Shape.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"
#include "Ellipsoid.h"
#include "Scene.h"
#include "helpers.h"

#define M_PI	3.14159265358979323846

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

double RANDOM_COLORS[7][3] = {
	{0.0000,    0.4470,    0.7410},
	{0.8500,    0.3250,    0.0980},
	{0.9290,    0.6940,    0.1250},
	{0.4940,    0.1840,    0.5560},
	{0.4660,    0.6740,    0.1880},
	{0.3010,    0.7450,    0.9330},
	{0.6350,    0.0780,    0.1840},
};

shared_ptr<Scene> setup_scene(int scene_num) {
	vector<shared_ptr<Shape>> shapes;
	vector<shared_ptr<Light>> lights;

	auto red = make_shared<Material>(
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.5f),
		100.0f,
		false
	);

	auto green = make_shared<Material>(
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.5f),
		100.0f,
		false
	);

	auto blue = make_shared<Material>(
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 0.5f),
		100.0f,
		false
	);

	auto black = make_shared<Material>(
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.0f,
		false
	);

	auto reflect = make_shared<Material>(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.0f,
		true
	);

	if (scene_num == 1 || scene_num == 2) {
		auto red_sphere = make_shared<Sphere>(glm::vec3(-0.5f, -1.0f, 1.0f), 1.0f, red, "red_sphere");
		auto green_sphere = make_shared<Sphere>(glm::vec3(0.5f, -1.0f, -1.0f), 1.0f, green, "green_sphere");
		auto blue_sphere = make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, blue, "blue_sphere");

		auto light = make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f), 1.0f);

		shapes.insert(shapes.end(), { red_sphere, green_sphere, blue_sphere });
		lights.insert(lights.end(), { light });
	}

	if (scene_num == 3) {
		auto red_ellipsoid = make_shared<Ellipsoid>(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.5f, 0.6f, 0.2f), red, "red_ellipsoid");
		auto green_sphere = make_shared<Sphere>(glm::vec3(-0.5f, 0.0f, -0.5f), 1.0f, green, "green_sphere");
		auto plane = make_shared<Plane>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), black, "plane");

		auto light1 = make_shared<Light>(glm::vec3(1.0f, 2.0f, 2.0f), 0.5f);
		auto light2 = make_shared<Light>(glm::vec3(-1.0f, 2.0f, -1.0f), 0.5f);

		shapes.insert(shapes.end(), { green_sphere, red_ellipsoid, plane });
		lights.insert(lights.end(), { light1, light2 });
	}
	if (scene_num == 4 || scene_num == 5) {
		auto red_sphere = make_shared<Sphere>(glm::vec3(0.5f, -0.7f, 0.5f), 0.3f, red, "red_sphere");
		auto blue_sphere = make_shared<Sphere>(glm::vec3(1.0f, -0.7f, 0.0f), 0.3f, green, "blue_sphere");
		auto floor = make_shared<Plane>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), black, "floor");
		auto black_wall = make_shared<Plane>(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f), black, "black_wall");
		auto reflective_sphere_1 = make_shared<Sphere>(glm::vec3(-0.5f, 0.0f, -0.5f), 1.0f, reflect, "reflective_sphere_1");
		auto reflective_sphere_2 = make_shared<Sphere>(glm::vec3(1.5f, 0.0f, -1.5f), 1.0f, reflect, "reflective_sphere_2");

		auto light1 = make_shared<Light>(glm::vec3(-1.0f, 2.0f, 1.0f), 0.5f);
		auto light2 = make_shared<Light>(glm::vec3(0.5f, -0.5f, 0.0f), 0.5f);

		shapes.insert(shapes.end(), { red_sphere, blue_sphere, floor, black_wall, reflective_sphere_1, reflective_sphere_2 });
		lights.insert(lights.end(), { light1, light2 });
	}

	auto scene = make_shared<Scene>(shapes, lights);
	return scene;
}

glm::vec3 compute_ray_color(shared_ptr<Scene> scene, Camera& cam, Ray& ray, float start_t, float end_t, int depth) {
	glm::vec3 color(0.0f, 0.0f, 0.0f);
	shared_ptr<Hit> hit = scene->hit(ray, start_t, end_t);
	if (hit != nullptr) {
		color += hit->material->ambient;

		glm::vec3 e = glm::normalize(cam.eye - hit->x);
		for (auto light : scene->lights) {
			glm::vec3 l = normalize(light->position - hit->x);
			glm::vec3 h = glm::normalize(e + l);

			Ray light_ray(hit->x, l);
			bool pass = false;
			for (int i = 0; i < scene->shapes.size(); i++) {
				float light_dist = glm::length(light->position - hit->x);
				auto h = scene->shapes.at(i)->intersect(light_ray, 1e-4, light_dist);
				if (h != nullptr) {
					pass = true;
					break;
				}
			}

			if (!pass) {
				glm::vec3 cd = hit->material->diffuse * max(0.0f, glm::dot(hit->n, l));
				glm::vec3 cs = hit->material->specular * pow(max(0.0f, glm::dot(hit->n, h)), hit->material->exponent);

				color += light->intensity * (cd + cs);
			}
		}
		//color = 0.5f * hit->n + glm::vec3(0.5f, 0.5f, 0.5f);

		if (hit->material->reflective && depth < 7) {
			glm::vec3 ref_v = ray.v - (2 * glm::dot(ray.v, hit->n) * hit->n);
			Ray reflect(hit->x, ref_v);
			color += 0.7f * compute_ray_color(scene, cam, reflect, 1e-4, end_t, depth + 1);
		}
	}

	if (color.r > 1) { color.r = 1.0f; }
	if (color.g > 1) { color.g = 1.0f; }
	if (color.b > 1) { color.b = 1.0f; }

	return color;
}

int main(int argc, char **argv)
{
	//int scene_num = stoi(argv[1]);
	string filename = string(argv[4]);

	Camera cam(
		glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		45.0f * (M_PI / 180.0),
		1.0f,
		stoi(argv[2]),
		stoi(argv[3])
	);

	auto img = make_shared<Image>(cam.width, cam.height);

	auto scene = setup_scene(stoi(argv[1]));


	//auto plane = scene->shapes.at(0);
	//cout << plane->position << endl;
	//Ray ray(
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, -1.0f, 0.0f)
	//);
	//auto hit = plane->intersect(ray, 0, numeric_limits<float>::infinity());
	//if (hit != nullptr) {
	//	cout << hit->x << endl;
	//}
	


	float world_height = 2 * cam.focal_dist * tan(cam.fovy / 2);
	float pixel_size = world_height / cam.height;

	for (int j = 0; j < cam.height; j++) {
		for (int i = 0; i < cam.width; i++) {
			float x = -cam.width / 2.0 + (i + 0.5);
			float y = -cam.height / 2.0 + (j + 0.5);

			glm::vec3 v = normalize(glm::vec3(pixel_size * x, pixel_size * y, -1.0));

			Ray r(cam.eye, v);

			glm::vec3 color = 255.0f * compute_ray_color(scene, cam, r, 0, numeric_limits<float>::infinity(), 0);
			img->setPixel(i, j, (int)color.r, (int)color.g, (int)color.b);
		}
	}

	img->writeToFile(filename);
	
	return 0;
}

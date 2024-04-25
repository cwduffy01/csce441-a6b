#include <iostream>
#include <string>
#include <limits>

#include <glm/gtc/matrix_transform.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Image.h"
#include "Scene.h"
#include "Camera.h"
#include "Material.h"
#include "Shape.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"
#include "Ellipsoid.h"
#include "Mesh.h"

#include "helpers.h"

#define M_PI	3.14159265358979323846

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;   

// load mesh geometry from obj file and store in buffer parameters
void load_mesh(string mesh_name, vector<float>& posBuf, vector<float>& norBuf, vector<float>& texBuf) {
	// Load geometry
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, mesh_name.c_str());
	if (!rc) {
		cerr << errStr << endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						norBuf.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuf.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuf.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}
					if (!attrib.texcoords.empty()) {
						texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}

// determine color of ray casted into scene
glm::vec3 compute_ray_color(shared_ptr<Scene> scene, shared_ptr<Camera> camera, Ray& ray, float start_t, float end_t, int depth) {
	glm::vec3 color(0.0f, 0.0f, 0.0f);	// bg color
	shared_ptr<Hit> hit = scene->hit(ray, start_t, end_t);	// get hit
	if (hit != nullptr) {	// if hit exists
		color += hit->material->ambient;

		// calculate blinn phong for each light
		glm::vec3 e = glm::normalize(camera->eye - hit->x);
		for (auto light : scene->lights) {
			glm::vec3 l = normalize(light->position - hit->x);
			glm::vec3 h = glm::normalize(e + l);

			// cast ray to light and detect shadow
			Ray light_ray(hit->x, l);
			bool shadow = false;
			for (int i = 0; i < scene->shapes.size(); i++) {
				float light_dist = glm::length(light->position - hit->x);
				auto h = scene->shapes.at(i)->intersect(light_ray, 1e-4, light_dist);
				if (h != nullptr) {
					shadow = true;
					break;
				}
			}

			// if light is hit by ray, compute diffuse and specular
			if (!shadow) {
				glm::vec3 cd = hit->material->diffuse * max(0.0f, glm::dot(hit->n, l));
				glm::vec3 cs = hit->material->specular * pow(max(0.0f, glm::dot(hit->n, h)), hit->material->exponent);

				color += light->intensity * (cd + cs);
			}
		}

		// recursion for reflective materials
		if (hit->material->reflective && depth < 7) {
			glm::vec3 ref_v = ray.v - (2 * glm::dot(ray.v, hit->n) * hit->n);
			Ray reflect(hit->x, ref_v);
			color += 0.7f * compute_ray_color(scene, camera, reflect, 1e-4, end_t, depth + 1);
		}
	}

	// cap r g and b values
	if (color.r > 1) { color.r = 1.0f; }
	if (color.g > 1) { color.g = 1.0f; }
	if (color.b > 1) { color.b = 1.0f; }

	return color;
}

// create scene based on scene number
shared_ptr<Scene> setup_scene(int scene_num) {
	vector<shared_ptr<Shape>> shapes;
	vector<shared_ptr<Light>> lights;

	// MATERIALS
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

	// MESHES
	vector<float> posBuf;
	vector<float> norBuf;
	vector<float> texBuf;

	load_mesh("../resources/bunny.obj", posBuf, norBuf, texBuf);

	if (scene_num == 1 || scene_num == 2 || scene_num == 8) {
		auto red_sphere = make_shared<Sphere>(glm::vec3(-0.5f, -1.0f, 1.0f), 1.0f, red, "red_sphere");
		auto green_sphere = make_shared<Sphere>(glm::vec3(0.5f, -1.0f, -1.0f), 1.0f, green, "green_sphere");
		auto blue_sphere = make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, blue, "blue_sphere");

		auto light = make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f), 1.0f);

		shapes.insert(shapes.end(), { red_sphere, green_sphere, blue_sphere });
		lights.insert(lights.end(), { light });
	}
	else if (scene_num == 3) {
		auto red_ellipsoid = make_shared<Ellipsoid>(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.5f, 0.6f, 0.2f), red, "red_ellipsoid");
		auto green_sphere = make_shared<Sphere>(glm::vec3(-0.5f, 0.0f, -0.5f), 1.0f, green, "green_sphere");
		auto plane = make_shared<Plane>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), black, "plane");

		auto light1 = make_shared<Light>(glm::vec3(1.0f, 2.0f, 2.0f), 0.5f);
		auto light2 = make_shared<Light>(glm::vec3(-1.0f, 2.0f, -1.0f), 0.5f);

		shapes.insert(shapes.end(), { green_sphere, red_ellipsoid, plane });
		lights.insert(lights.end(), { light1, light2 });
	}
	else if (scene_num == 4 || scene_num == 5) {
		auto red_sphere = make_shared<Sphere>(glm::vec3(0.5f, -0.7f, 0.5f), 0.3f, red, "red_sphere");
		auto blue_sphere = make_shared<Sphere>(glm::vec3(1.0f, -0.7f, 0.0f), 0.3f, blue, "blue_sphere");
		auto floor = make_shared<Plane>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), black, "floor");
		auto black_wall = make_shared<Plane>(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f), black, "black_wall");
		auto reflective_sphere_1 = make_shared<Sphere>(glm::vec3(-0.5f, 0.0f, -0.5f), 1.0f, reflect, "reflective_sphere_1");
		auto reflective_sphere_2 = make_shared<Sphere>(glm::vec3(1.5f, 0.0f, -1.5f), 1.0f, reflect, "reflective_sphere_2");

		auto light1 = make_shared<Light>(glm::vec3(-1.0f, 2.0f, 1.0f), 0.5f);
		auto light2 = make_shared<Light>(glm::vec3(0.5f, -0.5f, 0.0f), 0.5f);

		shapes.insert(shapes.end(), { red_sphere, blue_sphere, floor, black_wall, reflective_sphere_1, reflective_sphere_2 });
		lights.insert(lights.end(), { light1, light2 });
	}
	else if (scene_num == 6) {
		auto bunny = make_shared<Mesh>(posBuf, norBuf, texBuf, glm::vec3(0.0f, 0.0f, 0.0f), blue, "bunny");
		auto light = make_shared<Light>(glm::vec3(-1.0f, 1.0f, 1.0f), 1.0f);

		shapes.insert(shapes.end(), { bunny });
		lights.insert(lights.end(), { light });
	}
	else if (scene_num == 7) {
		glm::mat4 E(1.0f);

		E[0][0] = 1.5000;
		E[1][1] = 1.4095;
		E[1][2] = 0.5130;
		E[2][1] = -0.5130;
		E[2][2] = 1.4095;
		E[3][0] = 0.3000;
		E[3][1] = -1.5000;

		auto bunny = make_shared<Mesh>(posBuf, norBuf, texBuf, E, blue, "bunny");
		auto light = make_shared<Light>(glm::vec3(1.0f, 1.0f, 2.0f), 1.0f);

		shapes.insert(shapes.end(), { bunny });
		lights.insert(lights.end(), { light });
	}

	auto scene = make_shared<Scene>(shapes, lights);
	return scene;
}

// set up camera specs based on scene number
shared_ptr<Camera> setup_camera(int scene_num, int width, int height) {
	shared_ptr<Camera> camera;

	if (scene_num != 8) {
		camera = make_shared<Camera>(
			glm::vec3(0.0f, 0.0f, 5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			45.0f * (M_PI / 180.0),
			1.0f,
			width,
			height
		);
	}
	else if (scene_num == 8) {
		camera = make_shared<Camera>(
			glm::vec3(-3.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			60.0f * (M_PI / 180.0),
			1.0f,
			width,
			height
		);
	}

	return camera;
}

int main(int argc, char **argv)
{
	// get arguments
	int scene_num = stoi(argv[1]);
	int width = stoi(argv[2]);
	int height = stoi(argv[3]);
	string filename = string(argv[4]);

	// create objects
	auto img = make_shared<Image>(width, height);
	auto scene = setup_scene(scene_num);
	auto camera = setup_camera(scene_num, width, height);

	// basic parameters
	float world_height = 2 * camera->focal_dist * tan(camera->fovy / 2);
	float pixel_size = world_height / camera->height;

	// iterate through image pixels
	for (int j = 0; j < camera->height; j++) {
		for (int i = 0; i < camera->width; i++) {
			float x = -camera->width / 2.0 + (i + 0.5);
			float y = -camera->height / 2.0 + (j + 0.5);

			// get ray direction in world coordinates based on camera specs
			glm::vec3 v = glm::normalize(camera->center - camera->eye) * camera->focal_dist;
			v += pixel_size * y * glm::normalize(camera->up);
			glm::vec3 right = glm::cross((camera->center - camera->eye), camera->up);
			v += pixel_size * x * glm::normalize(right);

			// create ray and get ray's color
			Ray r(camera->eye, normalize(v));
			glm::vec3 color = 255.0f * compute_ray_color(scene, camera, r, 0, numeric_limits<float>::infinity(), 0);
			
			// set pixel color to ray color
			img->setPixel(i, j, (int)color.r, (int)color.g, (int)color.b);
		}
	}

	img->writeToFile(filename);
	return 0;
}

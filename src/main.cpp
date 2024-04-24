#include <iostream>
#include <string>
#include <limits>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Image.h"
#include "Material.h"
#include "Shape.h"
#include "Light.h"
#include "Sphere.h"
#include "Scene.h"
#include "helpers.h"

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

	if (scene_num == 1) {
		auto red_sphere = make_shared<Sphere>(glm::vec3(-0.5f, -1.0f, 1.0f), 1.0f, red, "red_sphere");
		auto green_sphere = make_shared<Sphere>(glm::vec3(0.5f, -1.0f, -1.0f), 1.0f, green, "green_sphere");
		auto blue_sphere = make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, blue, "blue_sphere");

		auto light = make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f), 1.0f);

		shapes.insert(shapes.end(), { red_sphere, green_sphere, blue_sphere });
		lights.insert(lights.end(), { light });
	}

	auto scene = make_shared<Scene>(shapes, lights);
	return scene;
}

int main(int argc, char **argv)
{
	//if(argc < 2) {
	//	cout << "Usage: A1 meshfile" << endl;
	//	return 0;
	//}
	//string meshName(argv[1]);

	
	//auto shape = make_shared<Shape>(
	//	glm::vec3(1.0f, 1.0f, 1.0f),
	//	mat,
	//	"benson"
	//);
	//shape->name = "bee";

	//auto shape = make_shared<Sphere>(
	//	glm::vec3(1.0f, 1.0f, 1.0f),
	//	3.0f,
	//	mat,
	//	"hi"
	//);

	//vector<shared_ptr<Shape>> shapes;

	//auto shape1 = make_shared<Sphere>();
	//shape1->name = "sphere1";

	//auto shape2 = make_shared<Sphere>();
	//shape2->position = glm::vec3(0.0f, 0.0f, -3.0f);
	//shape2->radius = 2.0f;
	//shape2->name = "sphere2";

	//shapes.insert(shapes.end(), { shape1 });

	//Ray r(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -5.0f));

	//for (shared_ptr<Shape> shape : shapes) {
	//	auto h = shape->intersect(r, 0, 10);
	//	if (h != nullptr) {
	//		cout << h->name << ", " << h->t << endl;
	//	}
	//}

	auto scene = setup_scene(1);

	for (auto s : scene->shapes) {
		cout << s->name << endl;
	}
	//cout << s->shapes.at(0)->name << endl;
	
	return 0;
}

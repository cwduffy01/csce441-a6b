#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <limits>
#include "Material.h"
#include "Hit.h"
#include "Ray.h"

using namespace std;

class Shape {
public:
	glm::vec3 position;
	shared_ptr<Material> material;
	string name;

	Shape();
	Shape(glm::vec3 position, shared_ptr<Material> material, string name);

	virtual shared_ptr<Hit> intersect(Ray& r, float start_t, float end_t) = 0;
};

#endif SHAPE_H
#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"

class Plane : public Shape {
public:
	glm::vec3 normal;

	Plane();
	Plane(glm::vec3 position, glm::vec3 normal, shared_ptr<Material> m, string name);

	shared_ptr<Hit> intersect(Ray& r, float start_t, float end_t);
};

#endif
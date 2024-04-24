#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	float radius;

	Sphere();
	Sphere(glm::vec3 position, float radius, shared_ptr<Material> m, string name);

	shared_ptr<Hit> intersect(Ray& r, float start_t, float end_t);


};

#endif
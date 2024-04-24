#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "Shape.h"

class Ellipsoid : public Shape {
public:
	glm::vec3 scale;
	glm::mat4 E;
	glm::mat4 E_inv;
	glm::mat4 E_inv_tr;

	Ellipsoid();
	Ellipsoid(glm::vec3 position, glm::vec3 scale, shared_ptr<Material> m, string name);

	shared_ptr<Hit> intersect(Ray& r, float start_t, float end_t);
};

#endif
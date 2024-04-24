#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray {
public:
	glm::vec3 p;
	glm::vec3 v;

	Ray();
	Ray(glm::vec3 p, glm::vec3 v);
};

#endif
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light {
public:
	glm::vec3 position;
	float intensity;

	Light();
	Light(glm::vec3 position, float intensity);
};

#endif
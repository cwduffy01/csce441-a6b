#ifndef HIT_H
#define HIT_H

#include <glm/glm.hpp>
#include "Material.h"

using namespace std;

class Hit
{
public:
	Hit() : x(0), n(0), t(0) {}
	Hit(const glm::vec3& x, const glm::vec3& n, float t, shared_ptr<Material> material, string name) { this->x = x; this->n = n; this->t = t; this->material = material; this->name = move(name); }
	glm::vec3 x; // position
	glm::vec3 n; // normal
	float t; // distance
	shared_ptr<Material> material;
	string name;
};

#endif
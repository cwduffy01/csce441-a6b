#include "Plane.h"
#include "helpers.h"

Plane::Plane() : Shape(), normal(0.0f, 1.0f, 0.0f) {}

Plane::Plane(glm::vec3 position, glm::vec3 normal, shared_ptr<Material> m, string name) :
	Shape(position, m, name),
	normal(glm::normalize(normal))
{}

shared_ptr<Hit> Plane::intersect(Ray& r, float start_t, float end_t) {

	float t = glm::dot(normal, position - r.p) / glm::dot(normal, r.v);

	if (t < start_t || t > end_t) { return nullptr; }

	glm::vec3 x = r.p + t * r.v;
	glm::vec3 n = normal;
	auto h = make_shared<Hit>(x, n, t, this->material, this->name);

	return h;
}
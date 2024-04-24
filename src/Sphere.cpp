#include "Sphere.h"

//#include <iostream>
#include "helpers.h"

Sphere::Sphere() : Shape(), radius(1.0f) {}

Sphere::Sphere(glm::vec3 position, float radius, shared_ptr<Material> m, string name) :
	Shape(position, m, name),
	radius(radius)
{}

shared_ptr<Hit> Sphere::intersect(Ray& r, float start_t, float end_t) {
	glm::vec3 pc = r.p - position;
	float a = glm::dot(r.v, r.v);
	float b = 2 * glm::dot(r.v, pc);
	float c = glm::dot(pc, pc) - pow(radius, 2);
	float d = pow(b, 2) - 4 * a * c;

	float t = -1.0;

	if (d > 0) {
		float t0 = (-b + sqrt(d)) / (2 * a);
		float t1 = (-b - sqrt(d)) / (2 * a);

		if (t0 < start_t || t0 > end_t) { t0 = numeric_limits<float>::infinity(); }
		if (t1 < start_t || t1 > end_t) { t1 = numeric_limits<float>::infinity(); }

		cout << "t0 = " << t0 << " : " << (r.p + t0 * r.v) << endl;
		cout << "t1 = " << t1 << " : " << (r.p + t1 * r.v) << endl;
		std::cout << std::endl;

		float t = fmin(t0, t1);
		if (t == numeric_limits<float>::infinity()) { return nullptr; }

		glm::vec3 x = r.p + t * r.v;
		glm::vec3 n = glm::normalize((x - position) / radius);

		auto h = make_shared<Hit>(x, n, t, this->material, this->name);

		return h;
	}
	return nullptr;
}
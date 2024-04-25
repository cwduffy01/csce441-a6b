#include "Ellipsoid.h"

Ellipsoid::Ellipsoid() : Shape(), scale(1.0f, 1.0f, 1.0f) {}

Ellipsoid::Ellipsoid(glm::vec3 position, glm::vec3 scale, shared_ptr<Material> m, string name) :
	Shape(position, m, name),
	scale(scale)
{
	// compute transformation matrix based on translation and scale
	glm::mat4 T(1.0f);
	glm::mat4 S(1.0f);

	T[3][0] = position.x;
	T[3][1] = position.y;
	T[3][2] = position.z;

	S[0][0] = scale.x;
	S[1][1] = scale.y;
	S[2][2] = scale.z;

	E = T * S;
	E_inv = glm::inverse(E);
	E_inv_tr = glm::transpose(E_inv);
}

shared_ptr<Hit> Ellipsoid::intersect(Ray& r, float start_t, float end_t) {
	// transform ray to local space
	glm::vec3 p_prime = glm::vec3(E_inv * glm::vec4(r.p, 1.0f));
	glm::vec3 v_prime = glm::vec3(E_inv * glm::vec4(r.v, 0.0f));

	// quadratic formula params
	float a = dot(v_prime, v_prime);
	float b = 2 * dot(v_prime, p_prime);
	float c = dot(p_prime, p_prime) - 1;
	float d = pow(b, 2) - 4 * a * c;

	if (d > 0) {	// if intersection
		float t0_prime = (-b - sqrt(d)) / (2 * a);
		float t1_prime = (-b + sqrt(d)) / (2 * a);

		// set out-of-bounds t values to infinity
		if (t0_prime < start_t || t0_prime > end_t) { t0_prime = numeric_limits<float>::infinity(); }
		if (t1_prime < start_t || t1_prime > end_t) { t1_prime = numeric_limits<float>::infinity(); }

		float t_prime = fmin(t0_prime, t1_prime);
		if (t_prime == numeric_limits<float>::infinity()) { return nullptr; }	// exit if out of bounds
		glm::vec3 x_prime = p_prime + t_prime * v_prime;

		// transform back to world space
		glm::vec3 x = glm::vec3(E * glm::vec4(x_prime, 1.0f));
		glm::vec3 n = normalize(glm::vec3(E_inv_tr * glm::vec4(x_prime, 0.0f)));
		float t = length(x - r.p);

		// return hit
		auto h = make_shared<Hit>(x, n, t, material, name);
		return h;
	}
	return nullptr;
}
#include "Ellipsoid.h"

Ellipsoid::Ellipsoid() : Shape(), scale(1.0f, 1.0f, 1.0f) {}

Ellipsoid::Ellipsoid(glm::vec3 position, glm::vec3 scale, shared_ptr<Material> m, string name) :
	Shape(position, m, name),
	scale(scale)
{
	glm::mat4 T(1.0f);
	glm::mat4 R(1.0f);
	glm::mat4 S(1.0f);

	T[3][0] = position.x;
	T[3][1] = position.y;
	T[3][2] = position.z;

	S[0][0] = scale.x;
	S[1][1] = scale.y;
	S[2][2] = scale.z;

	E = T * R * S;
	E_inv = glm::inverse(E);
	E_inv_tr = glm::transpose(E_inv);
}

shared_ptr<Hit> Ellipsoid::intersect(Ray& r, float start_t, float end_t) {

	glm::vec3 p_prime = glm::vec3(E_inv * glm::vec4(r.p, 1.0f));
	glm::vec3 v_prime = glm::vec3(E_inv * glm::vec4(r.v, 0.0f));

	float a = glm::dot(v_prime, v_prime);
	float b = 2 * glm::dot(v_prime, p_prime);
	float c = glm::dot(p_prime, p_prime) - 1;
	float d = pow(b, 2) - 4 * a * c;

	if (d > 0) {
		float t0_prime = (-b + sqrt(d)) / (2 * a);
		glm::vec3 x0_prime = p_prime + t0_prime * v_prime;
		glm::vec3 x0 = glm::vec3(E * glm::vec4(x0_prime, 1.0f));
		glm::vec3 n0 = glm::normalize(glm::vec3(E_inv_tr * glm::vec4(v_prime, 0.0f)));
		float t0 = glm::length(x0 - r.p);
		if (glm::dot(r.v, x0 - r.p) < 0) { t0 = -t0; }

		float t1_prime = (-b - sqrt(d)) / (2 * a);
		glm::vec3 x1_prime = p_prime + t1_prime * v_prime;
		glm::vec3 x1 = glm::vec3(E * glm::vec4(x1_prime, 1.0f));
		glm::vec3 n1 = glm::normalize(glm::vec3(E_inv_tr * glm::vec4(v_prime, 0.0f)));
		float t1 = glm::length(x1 - r.p);
		if (glm::dot(r.v, x1 - r.p) < 0) { t1 = -t1; }

		if (t0 < start_t || t0 > end_t) { t0 = numeric_limits<float>::infinity(); }
		if (t1 < start_t || t1 > end_t) { t1 = numeric_limits<float>::infinity(); }

		shared_ptr<Hit> h;

		if (t0 == numeric_limits<float>::infinity() && t1 == numeric_limits<float>::infinity()) { return nullptr; }
		if (t0 < t1) { 
			return make_shared<Hit>(x0, n0, t0, material, name); 
		}
		else { 
			return make_shared<Hit>(x1, n1, t1, material, name); 
		}

		/*return h;*/
	}
	return nullptr;
}
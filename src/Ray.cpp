#include "Ray.h"

Ray::Ray() : p(0.0f, 0.0f, 0.0f), v(0.0f, 0.0f, -1.0f) {}

Ray::Ray(glm::vec3 p, glm::vec3 v) : p(p), v(glm::normalize(v)) {}
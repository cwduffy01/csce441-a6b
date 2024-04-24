#include "Light.h"

Light::Light() : position(0.0f, 0.0f, 0.0f), intensity(1.0f) {}

Light::Light(glm::vec3 position, float intensity) : position(position), intensity(intensity) {}
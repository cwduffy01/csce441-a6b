#include "helpers.h"

ostream& operator<<(ostream& os, const glm::vec3& v) {
	os << "(\t" << v.x << "\t" << v.y << "\t" << v.z << "\t)";
	return os;
}

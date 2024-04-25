#include "helpers.h"

ostream& operator<<(ostream& os, const glm::vec3& v) {
	os << "(\t" << v.x << "\t" << v.y << "\t" << v.z << "\t)";
	return os;
}

ostream& operator<<(ostream& os, const glm::vec4& v) {
	os << "(\t" << v.x << "\t" << v.y << "\t" << v.z << "\t" << v.w << "\t)";
	return os;
}

ostream& operator<<(ostream& os, const glm::mat4& M) {
	glm::mat4 MT = glm::transpose(M);
	os << MT * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) << endl;
	os << MT * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) << endl;
	os << MT * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) << endl;
	os << MT * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) << endl;
	return os;
}
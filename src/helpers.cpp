#include "helpers.h"

ostream& operator<<(ostream& os, const glm::vec3& v) {
	os << "(\t" << v.x << "\t" << v.y << "\t" << v.z << "\t)";
	return os;
}


//ostream& operator<<(ostream& os, const Hit& hit) {
//	os << "x = " << hit.x << endl << "n = " << hit.n << endl << "t = " << hit.t << endl << "name = " << hit.name << endl;
//}
#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <glm/glm.hpp>
using namespace std;

ostream& operator<<(ostream& os, const glm::vec3& v);
ostream& operator<<(ostream& os, const glm::mat4& M);

#endif
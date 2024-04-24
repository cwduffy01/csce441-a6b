#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

using namespace std;

class Camera {
public:
	glm::vec3 eye;
	glm::vec3 lookat;
	glm::vec3 up;
	float fovy;
	float focal_dist;
	int width;
	int height;

	Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up, float fovy, float focal_dist, int width, int height);
};

#endif
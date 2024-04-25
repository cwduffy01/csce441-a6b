#include "Camera.h"

Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up, float fovy, float focal_dist, int width, int height) :
	eye(eye),
	center(center),
	up(up),
	fovy(fovy),
	focal_dist(focal_dist),
	width(width),
	height(height)
{}
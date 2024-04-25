#include "Scene.h"

Scene::Scene() {}

Scene::Scene(vector<shared_ptr<Shape>>& shapes, vector<shared_ptr<Light>>& lights) : shapes(shapes), lights(lights) {}

// returns hit closest to camera with shape's material info
shared_ptr<Hit> Scene::hit(Ray& ray, float start_t, float end_t) {
	float min_t = numeric_limits<float>::infinity();
	shared_ptr<Hit> rtn = nullptr;

	// check ray intersections for each hit
	for (int i = 0; i < shapes.size(); i++) {
		auto h = shapes.at(i)->intersect(ray, start_t, end_t);
		// keep hit if it's closer to camera
		if (h != nullptr && h->t < min_t) {
			min_t = h->t;
			rtn = h;
		}
	}

	return rtn;
}
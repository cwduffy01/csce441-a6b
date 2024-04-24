#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "Shape.h"
#include "Light.h"

class Scene {
public:
	vector<shared_ptr<Shape>> shapes;
	vector<shared_ptr<Light>> lights;

	Scene();
	Scene(vector<shared_ptr<Shape>>& shapes, vector<shared_ptr<Light>>& lights);
};

#endif
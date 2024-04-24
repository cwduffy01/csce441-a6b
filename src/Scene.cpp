#include "Scene.h"

Scene::Scene() {}

Scene::Scene(vector<shared_ptr<Shape>>& shapes, vector<shared_ptr<Light>>& lights) : shapes(shapes), lights(lights) {}
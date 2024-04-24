#include "Shape.h"

Shape::Shape() :
	position(0.0f, 0.0f, 0.0f),
	material(make_shared<Material>()),
	name("barry")
{};

Shape::Shape(glm::vec3 position, shared_ptr<Material> material, string name) :
	position(position),
	material(material),
	name(move(name))
{}
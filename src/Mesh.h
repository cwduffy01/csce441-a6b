#ifndef MESH_H
#define MESH_H

#include "Shape.h"

class Mesh : public Shape {
public:
	vector<float> posBuf; // list of vertex positions
	vector<float> norBuf; // list of vertex normals
	vector<float> texBuf; // list of vertex texture coords

	// bounding sphere params
	glm::vec3 b_pos;
	float b_rad;

	// transformation matrices
	glm::mat4 E;
	glm::mat4 E_inv;
	glm::mat4 E_inv_tr;

	Mesh(vector<float>& posBuf, vector<float>& norBuf, vector<float>& texBuf);
	Mesh(vector<float>& posBuf, vector<float>& norBuf, vector<float>& texBuf, glm::vec3 position, shared_ptr<Material> material, string name);
	Mesh(vector<float>& posBuf, vector<float>& norBuf, vector<float>& texBuf, glm::mat4 E, shared_ptr<Material> material, string name);

	shared_ptr<Hit> intersect(Ray& r, float start_t, float end_t);

private:
	void set_bounding_sphere();
};

#endif 

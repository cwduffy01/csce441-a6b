#include "Mesh.h"

void Mesh::set_bounding_sphere() {
    glm::vec3 bbox[2];

    bbox[0] = glm::vec3(posBuf.at(0), posBuf.at(1), posBuf.at(2));
    bbox[1] = glm::vec3(posBuf.at(0), posBuf.at(1), posBuf.at(2));

    for (int i = 0; i < posBuf.size(); i += 3) {
        float x = posBuf.at(i);
        float y = posBuf.at(i + 1);
        float z = posBuf.at(i + 2);

        if (x < bbox[0].x) { bbox[0].x = x; }
        if (y < bbox[0].y) { bbox[0].y = y; }
        if (z < bbox[0].z) { bbox[0].z = z; }

        if (x > bbox[1].x) { bbox[1].x = x; }
        if (y > bbox[1].y) { bbox[1].y = y; }
        if (z > bbox[1].z) { bbox[1].z = z; }
    }

    b_pos = glm::vec3((bbox[0].x + bbox[1].x) / 2, (bbox[0].y + bbox[1].y) / 2, (bbox[0].z + bbox[1].z) / 2);
    b_rad = length(bbox[0] - b_pos);
}

Mesh::Mesh(vector<float>& posBuf, vector<float>& norBuf, vector<float>& texBuf) : 
	Shape(),
	posBuf(posBuf),
	norBuf(norBuf),
	texBuf(texBuf),
    E(1.0f)
{
    set_bounding_sphere();

    E_inv = glm::inverse(E);
    E_inv_tr = glm::transpose(E_inv);
}

Mesh::Mesh(vector<float>& posBuf, vector<float>& norBuf, vector<float>& texBuf, glm::vec3 position, shared_ptr<Material> material, string name) :
    Shape(position, material, name),
    posBuf(posBuf),
    norBuf(norBuf),
    texBuf(texBuf),
    E(1.0f)
{
    set_bounding_sphere();

    E[3][0] = position.x;
    E[3][1] = position.y;
    E[3][2] = position.z;

    E_inv = glm::inverse(E);
    E_inv_tr = glm::transpose(E_inv);
}

Mesh::Mesh(vector<float>& posBuf, vector<float>& norBuf, vector<float>& texBuf, glm::mat4 E, shared_ptr<Material> material, string name) :
    Shape(glm::vec3(E * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), material, name),
    posBuf(posBuf),
    norBuf(norBuf),
    texBuf(texBuf),
    E(E)
{
    set_bounding_sphere();

    E_inv = glm::inverse(E);
    E_inv_tr = glm::transpose(E_inv);
}


/* Ray-Triangle Intersection Test Routines          */
/* Different optimizations of my and Ben Trumbore's */
/* code from journals of graphics tools (JGT)       */
/* http://www.acm.org/jgt/                          */
/* by Tomas Moller, May 2000                        */

/*
Copyright 2020 Tomas Akenine-Möller

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <math.h>

#define EPSILON 0.000001
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2];

/* code rewritten to do tests on the sign of the determinant */
/* the division is before the test of the sign of the det    */
/* and one CROSS has been moved out from the if-else if-else */
int intersect_triangle3(double orig[3], double dir[3],
    double vert0[3], double vert1[3], double vert2[3],
    double* t, double* u, double* v)
{
    double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
    double det, inv_det;

    /* find vectors for two edges sharing vert0 */
    SUB(edge1, vert1, vert0);
    SUB(edge2, vert2, vert0);

    /* begin calculating determinant - also used to calculate U parameter */
    CROSS(pvec, dir, edge2);

    /* if determinant is near zero, ray lies in plane of triangle */
    det = DOT(edge1, pvec);

    /* calculate distance from vert0 to ray origin */
    SUB(tvec, orig, vert0);
    inv_det = 1.0 / det;

    CROSS(qvec, tvec, edge1);

    if (det > EPSILON)
    {
        *u = DOT(tvec, pvec);
        if (*u < 0.0 || *u > det)
            return 0;

        /* calculate V parameter and test bounds */
        *v = DOT(dir, qvec);
        if (*v < 0.0 || *u + *v > det)
            return 0;

    }
    else if (det < -EPSILON)
    {
        /* calculate U parameter and test bounds */
        *u = DOT(tvec, pvec);
        if (*u > 0.0 || *u < det)
            return 0;

        /* calculate V parameter and test bounds */
        *v = DOT(dir, qvec);
        if (*v > 0.0 || *u + *v < det)
            return 0;
    }
    else return 0;  /* ray is parallell to the plane of the triangle */

    *t = DOT(edge2, qvec) * inv_det;
    (*u) *= inv_det;
    (*v) *= inv_det;

    return 1;
}


shared_ptr<Hit> Mesh::intersect(Ray& r, float start_t, float end_t) {

    glm::vec3 p_prime = E_inv * glm::vec4(r.p, 1.0f);
    glm::vec3 v_prime = glm::normalize(E_inv * glm::vec4(r.v, 0.0f));

    glm::vec3 pc = p_prime - b_pos;
    float a = glm::dot(v_prime, v_prime);
    float b = 2 * glm::dot(v_prime, pc);
    float c = glm::dot(pc, pc) - pow(b_rad, 2);
    float d = pow(b, 2) - 4 * a * c;

    float t_prime = -1.0;

    if (d > 0) {
        float t0 = (-b + sqrt(d)) / (2 * a);
        float t1 = (-b - sqrt(d)) / (2 * a);

        if (t0 < start_t || t0 > end_t) { t0 = numeric_limits<float>::infinity(); }
        if (t1 < start_t || t1 > end_t) { t1 = numeric_limits<float>::infinity(); }

        //cout << "t0 = " << t0 << " : " << (r.p + t0 * r.v) << endl;
        //cout << "t1 = " << t1 << " : " << (r.p + t1 * r.v) << endl;
        //std::cout << std::endl;

        float t_circ = fmin(t0, t1);
        if (t_circ == numeric_limits<float>::infinity()) { return nullptr; }

        float min_t_prime = numeric_limits<float>::infinity();

        glm::vec3 min_x_prime(0.0f, 0.0f, 0.0f);
        glm::vec3 min_n_prime(0.0f, 0.0f, 0.0f);

        double t_prime;
        double u;
        double v;

        for (int i = 0; i < posBuf.size(); i += 9) {
            double orig[3] = { p_prime.x, p_prime.y, p_prime.z };
            double dir[3]  = { v_prime.x, v_prime.y, v_prime.z };

            double vert0[3] = { posBuf.at(i + 0), posBuf.at(i + 1), posBuf.at(i + 2) };
            double vert1[3] = { posBuf.at(i + 3), posBuf.at(i + 4), posBuf.at(i + 5) };
            double vert2[3] = { posBuf.at(i + 6), posBuf.at(i + 7), posBuf.at(i + 8) };

            if (intersect_triangle3(orig, dir, vert0, vert1, vert2, &t_prime, &u, &v) == 1 && t_prime < min_t_prime) {
                min_t_prime = (float)t_prime;

                min_x_prime = p_prime + (float)t_prime * v_prime;

                glm::vec3 n0(norBuf.at(i + 0), norBuf.at(i + 1), norBuf.at(i + 2));
                glm::vec3 n1(norBuf.at(i + 3), norBuf.at(i + 4), norBuf.at(i + 5));
                glm::vec3 n2(norBuf.at(i + 6), norBuf.at(i + 7), norBuf.at(i + 8));

                min_n_prime = glm::normalize((float)(1 - u - v) * n0 + (float)u * n1 + (float)v * n2);
            }
        }

        if (min_t_prime == numeric_limits<float>::infinity() || min_t_prime < start_t || min_t_prime > end_t) {
            return nullptr;
        }

        //glm::vec3 x = glm::vec3(E * 

        glm::vec3 x = glm::vec3(E * glm::vec4(min_x_prime, 1.0f));
        glm::vec3 n = normalize(glm::vec3(E_inv_tr * glm::vec4(min_n_prime, 0.0f)));
        float t = length(x - r.p);

        auto h = make_shared<Hit>(x, n, t, material, name);
        return h;
    }
    return nullptr;
}





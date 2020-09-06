#include "torus.h"

#include <math.h>
#include <vector>
#include <GL\glew.h>
#include <iostream>

#define PI 3.14

Torus::Torus(int n_x, int n_y, float Rad, float rad) : num_x(n_x), num_y(n_y), r(rad), R(Rad)
{
}

void Torus::initData()
{
    num_vertices = (num_x + 1) * (num_y + 1);
    restart_index = num_vertices;
    num_indices = (num_x * 2 * (num_y + 1)) + num_x - 1;

    // build vertices
    std::vector<float> vertices;

    float dTheta = 2 * PI / float(num_x);
    float dPhi = 2 * PI / float(num_y);

    float theta = 0.0f;

    for (int i = 0; i <= num_x; i++)
    {
        float phi = 0.0f;

        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int j = 0; j <= num_y; j++)
        {
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = (R + r * cosPhi)* cosTheta;
            float y = (R + r * cosPhi)* sinTheta;
            float z = r * sinPhi;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            phi += dPhi;
        }

        theta += dTheta;
    }

    // build indices
    std::vector<unsigned int> indices;
    GLuint index = 0;
    for (auto i = 0; i < num_x; i++)
    {
        for (auto j = 0; j <= num_y; j++)
        {
            indices.push_back(index);
            indices.push_back(index + num_y + 1);
            index++;
        }
        if (i != num_x - 1) {
            indices.push_back(restart_index);
      }
    }

    unsigned int index_b = (1+num_y)*num_x;
    index = 0;
    for (auto j = 0; j <= num_y; j++)
        {
            indices.push_back(index_b);
            indices.push_back(index + num_y + 1);
            index++;
            index_b++;
        }

    num_indices = indices.size();

    vao.create();
    vao.bind();
    VertexBuffer vbo(&vertices[0], vertices.size() * sizeof(float));
    VertexBufferLayout layout;
    layout.AddFloat(3);
    vao.addBuffer(vbo, layout);
    ibo.setData(&indices[0], indices.size());

}

void Torus::render(){

    vao.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    ibo.bind();

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(restart_index);
    glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT, (void *)(0));
    glDisable(GL_PRIMITIVE_RESTART);

}

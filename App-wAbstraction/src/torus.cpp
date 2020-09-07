#include "torus.h"

#include <math.h>
#include <vector>
#include <GL\glew.h>
#include <iostream>

#define PI 3.14

Torus::Torus(int n_x, int n_y, float Rad, float rad) : num_x(n_x), num_y(n_y), r(rad), R(Rad)
{}

void Torus::initData() {
    num_vertices = (num_x + 1) * (num_y + 1);
    restart_index = num_vertices;
    num_indices = (num_x * 2 * (num_y + 1)) + num_x - 1;

    std::vector<float> vertices, normales;

    float dTheta = 2 * PI / float(num_x);
    float dPhi = 2 * PI / float(num_y);
    float theta = 0.0f;

    for (int i = 0; i <= num_x; i++) {
        float phi = 0.0f;

        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int j = 0; j <= num_y; j++) {
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            vertices.push_back((R + r * cosPhi)* cosTheta); // x
            vertices.push_back((R + r * cosPhi)* sinTheta); // y
            vertices.push_back(r * sinPhi); // z

            normales.push_back(cosTheta * cosPhi); // x
            normales.push_back(sinTheta * cosPhi); // y
            normales.push_back(sinPhi); // z

            phi += dPhi;
        }

        theta += dTheta;
    }

    std::vector<float> normalLines;
    float longLine = 0.02f;
    for (int i = 0; i < normales.size(); i+=3) {
        normalLines.push_back(vertices[i]);
        normalLines.push_back(vertices[i+1]);
        normalLines.push_back(vertices[i+2]);

        normalLines.push_back(vertices[i] + normales[i] * longLine);
        normalLines.push_back(vertices[i+1] + normales[i+1] * longLine);
        normalLines.push_back(vertices[i+2] + normales[i+2] * longLine);
    }

    std::vector<unsigned int> indices;
    GLuint index = 0;
    for (auto i = 0; i < num_x; i++) {
        for (auto j = 0; j <= num_y; j++) {
            indices.push_back(index);
            indices.push_back(index + num_y + 1);
            index++;
        }
        if (i != num_x - 1)
            indices.push_back(restart_index);
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

    // vao for paint normals

    vao_2.create();
    vao.bind();
    VertexBuffer vbo_2;
    vbo_2.allocate(&normalLines[0], normalLines.size() * sizeof(float));
    VertexBufferLayout layout_2;
    layout_2.AddFloat(3);
    vao_2.addBuffer(vbo_2, layout_2);
    vao_2.unbind();

    // vao for paint fill torus

    vao.create();
    vao.bind();
    VertexBuffer vbo;
    vbo.allocate(&vertices[0], vertices.size() * sizeof(float));
    VertexBufferLayout layout;
    layout.AddFloat(3);
    vao.addBuffer(vbo, layout);
    vao.unbind();

    ibo.allocate(&indices[0], indices.size());

}

void Torus::renderFill(){
    vao.bind();
    ibo.bind();

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(restart_index);
    glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT, (void *)(0));
    glDisable(GL_PRIMITIVE_RESTART);
}

void Torus::renderNormals(){
    vao_2.bind();
    glDrawArrays(GL_LINES, 0, num_vertices*2);
}

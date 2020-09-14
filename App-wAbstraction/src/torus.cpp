#include "torus.h"

#include <math.h>
#include <vector>
#include <GL\glew.h>
#include <iostream>

#define PI 3.14

void Torus::initData()
{
    num_vertices = (seg_x + 1) * (seg_y + 1);
    restart_index = num_vertices;
    num_indices = (seg_x * 2 * (seg_y + 1)) + seg_x - 1;

    // build vertices and normals   
    std::vector<float> vertices, normales;

    float dTheta = 2 * PI / float(seg_x);
    float dPhi = 2 * PI / float(seg_y);

    float theta = 0.0f;

    for (int i = 0; i <= seg_x; i++)
    {
        float phi = 0.0f;

        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int j = 0; j <= seg_y; j++)
        {
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

    // concatenando vertices y normales
    std::vector<float> vert_and_norm;
    for (int i = 0; i < normales.size(); i+=3) {
        vert_and_norm.push_back(vertices[i]);
        vert_and_norm.push_back(vertices[i+1]);
        vert_and_norm.push_back(vertices[i+2]);

        vert_and_norm.push_back(normales[i]);
        vert_and_norm.push_back(normales[i+1]);
        vert_and_norm.push_back(normales[i+2]);
    }

    // build vertices to paint normals
    std::vector<float> lineas_de_normales;
    float long_linea = 0.024f;
    for (int i = 0; i < normales.size(); i+=3) {
        lineas_de_normales.push_back(vertices[i]);
        lineas_de_normales.push_back(vertices[i+1]);
        lineas_de_normales.push_back(vertices[i+2]);

        lineas_de_normales.push_back(vertices[i] + normales[i] * long_linea);
        lineas_de_normales.push_back(vertices[i+1] + normales[i+1] * long_linea);
        lineas_de_normales.push_back(vertices[i+2] + normales[i+2] * long_linea);
    }

    // build indices
    std::vector<unsigned int> indices;
    GLuint index = 0;
    for (auto i = 0; i < seg_x; i++)
    {
        for (auto j = 0; j <= seg_y; j++)
        {
            indices.push_back(index);
            indices.push_back(index + seg_y + 1);
            index++;
        }
        if (i != seg_x - 1) {
            indices.push_back(restart_index);
      }
    }

    unsigned int index_b = (1+seg_y)*seg_x;
    index = 0;
    for (auto j = 0; j <= seg_y; j++)
        {
            indices.push_back(index_b);
            indices.push_back(index + seg_y + 1);
            index++;
            index_b++;
        }

    num_indices = indices.size();

    // vao for paint normals
    vao_2.create();
    vao_2.bind();
    VertexBuffer vbo_2;
    vbo_2.allocate(&lineas_de_normales[0], lineas_de_normales.size() * sizeof(float));
    VertexBufferLayout layout_2;
    layout_2.AddFloat(3);
    vao_2.addBuffer(vbo_2, layout_2);
    vao_2.unbind();

    // vao for paint torus (vertices and normals)
    vao.create();
    vao.bind();
    vbo.allocate(&vert_and_norm[0], vert_and_norm.size() * sizeof(float));
    VertexBufferLayout layout;
    layout.AddFloat(3); // vertices
    layout.AddFloat(3); // normales
    vao.addBuffer(vbo, layout);
    vao.unbind();

    ibo.allocate(&indices[0], indices.size());

}

void Torus::renderFill() const{

    // draw torus fill

    vao.bind();
    ibo.bind();

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(restart_index);
    glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT, (void *)(0));
    glDisable(GL_PRIMITIVE_RESTART);
}

void Torus::renderNormals() const{

    // draw torus normals

    vao_2.bind();
    glDrawArrays(GL_LINES, 0, num_vertices*2);
}

void Torus::renderWire() const{

    // draw "wire" mode
    
    vao.bind();
    ibo.bind();

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(restart_index);
    glDrawElements(GL_LINE_STRIP, num_indices, GL_UNSIGNED_INT, (void *)(0));
    glDisable(GL_PRIMITIVE_RESTART);
}

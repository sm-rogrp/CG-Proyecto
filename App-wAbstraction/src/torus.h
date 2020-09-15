#ifndef TORUS_H
#define TORUS_H

#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_buffer_layout.h"
#include "shape.h"

class Torus : public Shape
{

public:
    unsigned int num_vertices;
    unsigned int num_indices;
    unsigned int restart_index;
    float R, r;

    VertexArray vao;       // dibujar "fill" torus
    VertexBuffer vbo;      // vertices
    VertexBuffer vbo_norm; // normales
    IndexBuffer ibo;       // indices
    VertexArray vao_2;     // dibujar normales de torus

    Torus(int num_x, int num_y, float Rad, float rad) : Shape(num_x, num_y), R(Rad), r(rad) {}

    void setProp(float R, float r){
        this->R = R;
        this->r = r;
    }

    void initData();
    void renderFill() const;
    void renderNormals() const;
    void renderWire() const;

};

#endif

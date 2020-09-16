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
    VertexArray vao_2;     // dibujar normales de torus
    VertexBuffer vbo;      // vertices
    VertexBuffer vbo_2; // normales
    IndexBuffer ibo;       // indices

    Torus(int num_x, int num_y, float Rad, float rad) : Shape(num_x, num_y), R(Rad), r(rad) {}

    void setProp(float R, float r){
        this->R = R;
        this->r = r;
    }

    void initData();
    void renderFill() const;
    void renderNormals() const;
    void renderWire() const;

    void createVertexObjects(){
        vao_2.create();
        vao.create();
        vbo_2.create();
        vbo.create();
        ibo.create();
    }

};

#endif

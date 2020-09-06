#ifndef TORUS_H
#define TORUS_H

#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_buffer_layout.h"

class Torus{
public:
    unsigned int num_vertices;
    unsigned int num_indices;
    unsigned int num_x;
    unsigned int num_y;
    unsigned int restart_index;

    VertexArray vao;
    VertexArray vao_2; // dibujar normales
    IndexBuffer ibo;

    float R, r;

    Torus() {}
    Torus(int num_x, int num_y, float R, float r);
    
    void initData();
    void renderFill();
    void renderNormals();
};

#endif

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

    // VertexArray vao;
    // IndexBuffer ibo;
    unsigned int vao[1];
    unsigned int vbo[2]; // vbo 1: vertices, vbo 2: normales
    unsigned int ebo[1];  

    float R, r;

    Torus() {}
    Torus(int num_x, int num_y, float R, float r);
    
    void initData();
    void render();
};

#endif
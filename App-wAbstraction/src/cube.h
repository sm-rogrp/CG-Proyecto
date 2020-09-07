#ifndef CUBE_H
#define CUBE_H

#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_buffer_layout.h"
#include "shape.h"

class Cube : public Shape{
    
public:

    unsigned int num_vertices;
    unsigned int num_indices;
    unsigned int restart_index;

    VertexArray vao_fill;  
    VertexBuffer vbo_fill;  
    VertexArray vao_wire;  
    VertexBuffer vbo_wire; 
    IndexBuffer ibo; // indices
    VertexArray vao_2; // dibujar normales de Cube

    float side_len;

    Cube(int num_x, int num_y, float side_length): Shape(num_x, num_y), side_len(side_length) {};

    void initData();
    void renderFill();
    void renderNormals();
    void renderWire();
};

#endif
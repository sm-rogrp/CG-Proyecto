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
<<<<<<< HEAD
    void renderFill() const;
    void renderNormals() const;
    void renderWire() const;
=======
    void renderFill();
    void renderNormals();
    void renderWire();
>>>>>>> e5cea100a38e8a45e7d50db9d6736088e808b4d4
};

#endif
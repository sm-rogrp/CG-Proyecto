#include "cube.h"


void Cube::initData()
{
    std::vector<float> vertices;
    // float radio = 0.4f;
    float r = side_len/2;
    float step_x = side_len / seg_x;
    float step_y = side_len / seg_y;

    for (float i = -r; i <= r; i += step_x)
    {
        for (float j = -r; j <= r; j += step_y)
        {
            for (float k = -r; k <= r; k += step_x)
            {
                vertices.push_back(i);
                vertices.push_back(j);
                vertices.push_back(k);


                // normales
                // if (i == )
            }
        }
    }

    num_vertices = vertices.size() / 3;

    // indices cube
    std::vector<float> cube_fill{
        -r, r, -r,
        -r, r, r,
        r, r, -r,
        -r, r, r,
        r, r, -r,
        r, r, r,

        -r, r, r,
        r, r, r,
        -r, -r, r,
        r, r, r,
        -r, -r, r,
        r, -r, r,

        r, r, r,
        r, -r, r,
        r, r, -r,
        r, -r, r,
        r, r, -r,
        r, -r, -r,

        -r, -r, -r,
        -r, -r, r,
        r, -r, -r,
        -r, -r, r,
        r, -r, -r,
        r, -r, r,

        -r, r, -r,
        r, r, -r,
        -r, -r, -r,
        r, r, -r,
        -r, -r, -r,
        r, -r, -r,

        -r, -r, r,
        -r, -r, -r,
        -r, r, r,
        -r, -r, -r,
        -r, r, r,
        -r, r, -r
    };

    vao_fill.create();
    vbo_fill.allocate(&cube_fill[0], cube_fill.size() * sizeof(float));
    VertexBufferLayout layout_1;
    layout_1.AddFloat(3);
    vao_fill.addBuffer(vbo_fill, layout_1);

    vao_wire.bind();
    vbo_wire.allocate(&vertices[0], vertices.size() * sizeof(float));
    VertexBufferLayout layout_2;
    layout_2.AddFloat(3);
    vao_wire.addBuffer(vbo_wire, layout_2);
}

<<<<<<< HEAD
void Cube::renderFill ()const
=======
void Cube::renderFill()
>>>>>>> e5cea100a38e8a45e7d50db9d6736088e808b4d4
{
    vao_fill.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

<<<<<<< HEAD
void Cube::renderWire () const{
=======
void Cube::renderWire(){
>>>>>>> e5cea100a38e8a45e7d50db9d6736088e808b4d4
    vao_wire.bind();
    // glDrawArrays(GL_LINE_STRIP, 0, )
}

<<<<<<< HEAD
void Cube::renderNormals()const {
=======
void Cube::renderNormals(){
>>>>>>> e5cea100a38e8a45e7d50db9d6736088e808b4d4

}
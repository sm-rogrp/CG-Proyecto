#ifndef SPHERE_H
#define SPHERE_H


#include <GL\glew.h>
#include "shape.h"
#include <vector>

class Sphere: public Shape {
public:
    // ctor/dtor
    Sphere(float radius, int sectors, int stacks, bool smooth);
    ~Sphere() {}

    void setSegments(int sectors, int stacks){
            this->sectorCount = sectors;
            this->stackCount = stacks;
    }

    void setProp(float rad){
        this->radius = rad;
    }


    // getters/setters
    float getRadius() const                 { return radius; }
    int getSectorCount() const              { return sectorCount; }
    int getStackCount() const               { return stackCount; }
    void set(float radius, int sectorCount, int stackCount, bool smooth=true);
    void setRadius(float radius);
    void setSectorCount(int sectorCount);
    void setStackCount(int stackCount);
    void setSmooth(bool smooth);

    void renderFill() const;                                  // draw surface
    void renderWire() const;     // draw lines only
    void renderNormals() const;

    // debug
    void printSelf() const;

protected:

public:
    // member functions
    void initData();
    void clearArrays();
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    std::vector<float> computeFaceNormal(float x1, float y1, float z1,
                                         float x2, float y2, float z2,
                                         float x3, float y3, float z3);

    // memeber vars
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    bool smooth;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    std::vector<float> norm_lines;


    unsigned int vbo_vert;
    unsigned int vbo_norm;
    unsigned int vbo_indi;
    unsigned int vbo_indi_lines;
    unsigned int vbo_norm_lines;
    unsigned int vao;
    unsigned int vao_2;

    void createVertexObjects(){
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo_vert);
        glGenBuffers(1, &vbo_norm);
        glGenBuffers(1, &vbo_indi);
        glGenBuffers(1, &vbo_indi_lines);
        glGenVertexArrays(1, &vao_2);
        glGenBuffers(1, &vbo_norm_lines);
    }

};

#endif
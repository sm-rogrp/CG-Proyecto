#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <GL\glew.h>
#include "shape.h"

class Cylinder : public Shape {
public:
    Cylinder(float baseRadius=1.0f, float topRadius=1.0f, float height=1.0f,
             int sectorCount=36, int stackCount=1, bool smooth=true);
    ~Cylinder() {}

    // funciones
    void set(float baseRadius, float topRadius, float height, int sectorCount, int stackCount, bool smooth=true);
    void renderFill() const;     
    void renderWire() const;     
    void renderNormals() const; 

    void clearArrays();
    void initData();
    void buildUnitCircleVertices();
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    std::vector<float> getSideNormals(); 

    // ----------------------------------

    std::vector<float> unitCircleVertices;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    std::vector<float> norm_lines;

    unsigned int vao;
    unsigned int vao_2;
    unsigned int vbo_vert;
    unsigned int vbo_norm;
    unsigned int vbo_indi;
    unsigned int vbo_lines_indi;
    unsigned int vbo_lines_norm;

    float baseRadius;
    float topRadius;
    float height;
    int sectorCount;                        // # of slices
    int stackCount;                         // # of stacks
    unsigned int baseIndex;                 // starting index of base
    unsigned int topIndex;                  // starting index of top
    bool smooth;
    
    void setSegments(int sectorCount, int stackCount) {
        this->sectorCount = sectorCount;
        this->stackCount = stackCount;
    }
    
    void setProp(float baseRadius, float topRadius, float height){
        this->baseRadius = baseRadius;
        this->topRadius = topRadius;
        this->height = height;
    }

};

#endif
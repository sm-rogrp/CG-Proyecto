#include <iostream>
#include <cmath>
#include <iomanip>
#include "cylinder.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT  = 1;

Cylinder::Cylinder(float baseRadius, float topRadius, float height, int sectors, int stacks, bool smooth)
{
    set(baseRadius, topRadius, height, sectors, stacks, smooth);
}

void Cylinder::set(float baseRadius, float topRadius, float height, int sectors, int stacks, bool smooth)
{
    this->baseRadius = baseRadius;
    this->topRadius = topRadius;
    this->height = height;
    this->sectorCount = sectors;
    if(sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if(stacks < MIN_STACK_COUNT)
        this->stackCount = MIN_STACK_COUNT;
    this->smooth = smooth;

    // generate unit circle vertices first
    buildUnitCircleVertices();
}

void Cylinder::renderFill() const
{
    // interleaved array

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indi);

    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
}

void Cylinder::renderWire() const
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_lines_indi);

    glDrawElements(GL_LINES, (unsigned int)lineIndices.size(), GL_UNSIGNED_INT, 0);
}

void Cylinder::renderNormals() const
{
    glBindVertexArray(vao_2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_lines_norm);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINES, 0, (unsigned int)norm_lines.size()/3);
}

void Cylinder::clearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
    std::vector<float>().swap(norm_lines);
}


void Cylinder::initData()
{
    // build vertices of cylinder with smooth shading
    // where v: sector angle (0 <= v <= 360)

    // clear memory of prev arrays

    clearArrays();


    float x, y, z;                                  // vertex position
    //float s, t;                                     // texCoord
    float radius;                                   // radius for each stack

    // get normals for cylinder sides
    std::vector<float> sideNormals = getSideNormals();

    // put vertices of side cylinder to array by scaling unit circle
    for(int i = 0; i <= stackCount; ++i)
    {
        z = -(height * 0.5f) + (float)i / stackCount * height;      // vertex position z
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);     // lerp
        float t = 1.0f - (float)i / stackCount;   // top-to-bottom

        for(int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            x = unitCircleVertices[k];
            y = unitCircleVertices[k+1];
            addVertex(x * radius, y * radius, z);   // position
            addNormal(sideNormals[k], sideNormals[k+1], sideNormals[k+2]); // normal
            addTexCoord((float)j / sectorCount, t); // tex coord
        }
    }


    // remember where the base.top vertices start
    unsigned int baseVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of base of cylinder
    z = -height * 0.5f;
    addVertex(0, 0, z);
    addNormal(0, 0, -1);
    addTexCoord(0.5f, 0.5f);
    for(int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        addVertex(x * baseRadius, y * baseRadius, z);
        addNormal(0, 0, -1);
        addTexCoord(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f);    // flip horizontal
    }

    // remember where the base vertices start
    unsigned int topVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of top of cylinder
    z = height * 0.5f;
    addVertex(0, 0, z);
    addNormal(0, 0, 1);
    addTexCoord(0.5f, 0.5f);
    for(int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        addVertex(x * topRadius, y * topRadius, z);
        addNormal(0, 0, 1);
        addTexCoord(x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
    }

    // put indices for sides
    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // bebinning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 trianles per sector
            addIndices(k1, k1 + 1, k2);
            addIndices(k2, k1 + 1, k2 + 1);

            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            // horizontal lines
            lineIndices.push_back(k2);
            lineIndices.push_back(k2 + 1);
            if(i == 0)
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }


    // remember where the base indices start
    baseIndex = (unsigned int)indices.size();

    // put indices for base
    for(int i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if(i < (sectorCount - 1))
            addIndices(baseVertexIndex, k + 1, k);
        else    // last triangle
            addIndices(baseVertexIndex, baseVertexIndex + 1, k);
    }

    // remember where the base indices start
    topIndex = (unsigned int)indices.size();

    for(int i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if(i < (sectorCount - 1))
            addIndices(topVertexIndex, k, k + 1);
        else
            addIndices(topVertexIndex, k, topVertexIndex + 1);
    }

    // build vertices to paint normals
    float long_linea = 0.012f;
    for (int i = 0; i < normals.size(); i+=3) {
        norm_lines.push_back(vertices[i]);
        norm_lines.push_back(vertices[i+1]);
        norm_lines.push_back(vertices[i+2]);

        norm_lines.push_back(vertices[i] + normals[i] * long_linea);
        norm_lines.push_back(vertices[i+1] + normals[i+1] * long_linea);
        norm_lines.push_back(vertices[i+2] + normals[i+2] * long_linea);
    }

    /*
    TO DO: usar las clases creadas
    */

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo_vert);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_norm);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_indi);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indi);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_lines_indi);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_lines_indi);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, lineIndices.size() * sizeof(unsigned int), &lineIndices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_2);
    glBindVertexArray(vao_2);

    glGenBuffers(1, &vbo_lines_norm);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_lines_norm);
    glBufferData(GL_ARRAY_BUFFER, norm_lines.size() * sizeof(float), &norm_lines[0], GL_STATIC_DRAW);

}

void Cylinder::buildUnitCircleVertices()
{
    const float PI = acos(-1);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    std::vector<float>().swap(unitCircleVertices);
    for(int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }
}

void Cylinder::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void Cylinder::addNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

void Cylinder::addTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}

void Cylinder::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

std::vector<float> Cylinder::getSideNormals()
{
    const float PI = acos(-1);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    // compute the normal vector at 0 degree first
    // tanA = (baseRadius-topRadius) / height
    float zAngle = atan2(baseRadius - topRadius, height);
    float x0 = cos(zAngle);     // nx
    float y0 = 0;               // ny
    float z0 = sin(zAngle);     // nz

    // rotate (x0,y0,z0) per sector angle
    std::vector<float> normals;
    for(int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        normals.push_back(cos(sectorAngle)*x0 - sin(sectorAngle)*y0);   // nx
        normals.push_back(sin(sectorAngle)*x0 + cos(sectorAngle)*y0);   // ny
        normals.push_back(z0);  // nz
        /*
        //debug
        float nx = cos(sectorAngle)*x0 - sin(sectorAngle)*y0;
        float ny = sin(sectorAngle)*x0 + cos(sectorAngle)*y0;
        std::cout << "normal=(" << nx << ", " << ny << ", " << z0
                  << "), length=" << sqrtf(nx*nx + ny*ny + z0*z0) << std::endl;
        */
    }

    return normals;
}

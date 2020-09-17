
#include <iostream>
#include <iomanip>
#include <cmath>
#include <GL\glew.h>
#include "cubesphere.h"

Cubesphere::Cubesphere(float radius, int sub, bool smooth) : radius(radius), subdivision(sub), smooth(smooth), interleavedStride(32)
{
    vertexCountPerRow = (unsigned int)pow(2, sub) + 1;
    vertexCountPerFace = vertexCountPerRow * vertexCountPerRow;
}


void Cubesphere::initData()
{
    // generate unit-length verties in +X face
    std::vector<float> unitVertices = Cubesphere::getUnitPositiveX(vertexCountPerRow);

    // clear memory of prev arrays
    clearArrays();

    float x, y, z, s, t;
    int k = 0, k1, k2;

    // build +X face
    for(unsigned int i = 0; i < vertexCountPerRow; ++i)
    {
        k1 = i * vertexCountPerRow;     // index for curr row
        k2 = k1 + vertexCountPerRow;    // index for next row
        t = (float)i / (vertexCountPerRow - 1);

        for(unsigned int j = 0; j < vertexCountPerRow; ++j, k += 3, ++k1, ++k2)
        {
            x = unitVertices[k];
            y = unitVertices[k+1];
            z = unitVertices[k+2];
            s = (float)j / (vertexCountPerRow - 1);
            addVertex(x*radius, y*radius, z*radius);
            addNormal(x, y, z);
            addTexCoord(s, t);

            // add indices
            if(i < (vertexCountPerRow-1) && j < (vertexCountPerRow-1))
            {
                addIndices(k1, k2, k1+1);
                addIndices(k1+1, k2, k2+1);
                // lines: left and top
                lineIndices.push_back(k1);  // left
                lineIndices.push_back(k2);
                lineIndices.push_back(k1);  // top
                lineIndices.push_back(k1+1);
            }
        }

    }

    // array size and index for building next face
    unsigned int startIndex;                    // starting index for next face
    int vertexSize = (int)vertices.size();      // vertex array size of +X face
    int indexSize = (int)indices.size();        // index array size of +X face
    int lineIndexSize = (int)lineIndices.size(); // line index size of +X face

    // build -X face by negating x and z
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i], vertices[i+1], -vertices[i+2]);
        addTexCoord(texCoords[j], texCoords[j+1]);
        addNormal(-normals[i], normals[i+1], -normals[i+2]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    for(int i = 0; i < lineIndexSize; i += 4)
    {
        // left and bottom lines
        lineIndices.push_back(startIndex + lineIndices[i]);     // left
        lineIndices.push_back(startIndex + lineIndices[i+1]);
        lineIndices.push_back(startIndex + lineIndices[i+1]);  // bottom
        lineIndices.push_back(startIndex + lineIndices[i+1] + 1);
    }

    // build +Y face by swapping x=>y, y=>-z, z=>-x
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i+2], vertices[i], -vertices[i+1]);
        addTexCoord(texCoords[j], texCoords[j+1]);
        addNormal(-normals[i+2], normals[i], -normals[i+1]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    for(int i = 0; i < lineIndexSize; ++i)
    {
        // top and left lines (same as +X)
        lineIndices.push_back(startIndex + lineIndices[i]);
    }

    // build -Y face by swapping x=>-y, y=>z, z=>-x
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i+2], -vertices[i], vertices[i+1]);
        addTexCoord(texCoords[j], texCoords[j+1]);
        addNormal(-normals[i+2], -normals[i], normals[i+1]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    for(int i = 0; i < lineIndexSize; i += 4)
    {
        // top and right lines
        lineIndices.push_back(startIndex + lineIndices[i]); // top
        lineIndices.push_back(startIndex + lineIndices[i+3]);
        lineIndices.push_back(startIndex + lineIndices[i] + 1); // right
        lineIndices.push_back(startIndex + lineIndices[i+1] + 1);
    }

    // build +Z face by swapping x=>z, z=>-x
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i+2], vertices[i+1], vertices[i]);
        addTexCoord(texCoords[j], texCoords[j+1]);
        addNormal(-normals[i+2], normals[i+1], normals[i]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    for(int i = 0; i < lineIndexSize; ++i)
    {
        // top and left lines (same as +X)
        lineIndices.push_back(startIndex + lineIndices[i]);
    }

    // build -Z face by swapping x=>-z, z=>x
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(vertices[i+2], vertices[i+1], -vertices[i]);
        addTexCoord(texCoords[j], texCoords[j+1]);
        addNormal(normals[i+2], normals[i+1], -normals[i]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    for(int i = 0; i < lineIndexSize; i += 4)
    {
        // left and bottom lines
        lineIndices.push_back(startIndex + lineIndices[i]);     // left
        lineIndices.push_back(startIndex + lineIndices[i+1]);
        lineIndices.push_back(startIndex + lineIndices[i+1]);   // bottom
        lineIndices.push_back(startIndex + lineIndices[i+1] + 1);
    }

    // build vertices to paint normals
    float long_linea = 0.02f;
    for (int i = 0; i < normals.size(); i+=3) {
        norm_lines.push_back(vertices[i]);
        norm_lines.push_back(vertices[i+1]);
        norm_lines.push_back(vertices[i+2]);

        norm_lines.push_back(vertices[i] + normals[i] * long_linea);
        norm_lines.push_back(vertices[i+1] + normals[i+1] * long_linea);
        norm_lines.push_back(vertices[i+2] + normals[i+2] * long_linea);
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indi);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indi_lines);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, lineIndices.size() * sizeof(unsigned int), &lineIndices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_2);
    glBindVertexArray(vao_2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_norm_lines);
    glBufferData(GL_ARRAY_BUFFER, norm_lines.size() * sizeof(float), &norm_lines[0], GL_STATIC_DRAW);
}


void Cubesphere::renderFill() const {
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

void Cubesphere::renderWire() const {
    
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indi_lines);
    glDrawElements(GL_LINES, (unsigned int)lineIndices.size(), GL_UNSIGNED_INT, 0);

}

void Cubesphere::renderNormals() const {

    glBindVertexArray(vao_2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_norm_lines);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    // std::cout<<":::"<<norm_lines.size()<<std::endl;
    glDrawArrays(GL_LINES, 0, (unsigned int)norm_lines.size() / 3);
}

void Cubesphere::clearArrays() {
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<float>().swap(norm_lines);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}

void Cubesphere::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void Cubesphere::addVertices(const float v1[3], const float v2[3], const float v3[3], const float v4[3])
{
    vertices.insert(vertices.end(), v1, v1 + 3);    // v1
    vertices.insert(vertices.end(), v2, v2 + 3);    // v2
    vertices.insert(vertices.end(), v3, v3 + 3);    // v3
    vertices.insert(vertices.end(), v4, v4 + 3);    // v4
}

void Cubesphere::addNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

void Cubesphere::addNormals(const float n1[3], const float n2[3], const float n3[3], const float n4[3])
{
    normals.insert(normals.end(), n1, n1 + 3);  // n1
    normals.insert(normals.end(), n2, n2 + 3);  // n2
    normals.insert(normals.end(), n3, n3 + 3);  // n3
    normals.insert(normals.end(), n4, n4 + 3);  // n4
}


void Cubesphere::addTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}

void Cubesphere::addTexCoords(const float t1[2], const float t2[2], const float t3[2], const float t4[2])
{
    texCoords.insert(texCoords.end(), t1, t1 + 2);  // t1
    texCoords.insert(texCoords.end(), t2, t2 + 2);  // t2
    texCoords.insert(texCoords.end(), t3, t3 + 2);  // t3
    texCoords.insert(texCoords.end(), t4, t4 + 2);  // t4
}


void Cubesphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

const float* Cubesphere::getVerticesForFace(int faceId) const
{
    if(faceId >= 0 && faceId <= 5)
    {
        return &vertices[faceId * vertices.size() / 6];
    }
    else
    {
        // invalid ID, return the beginging of array
        return vertices.data();
    }
}

const float* Cubesphere::getNormalsForFace(int faceId) const
{
    if(faceId >= 0 && faceId <= 5)
    {
        return &normals[faceId * normals.size() / 6];
    }
    else
    {
        // invalid ID, return the beginging of array
        return normals.data();
    }
}

const float* Cubesphere::getTexCoordsForFace(int faceId) const
{
    if(faceId >= 0 && faceId <= 5)
    {
        return &texCoords[faceId * texCoords.size() / 6];
    }
    else
    {
        // invalid ID, return the beginging of array
        return texCoords.data();
    }
}

const float* Cubesphere::getInterleavedVerticesForFace(int faceId) const
{
    if(faceId >= 0 && faceId <= 5)
    {
        return &interleavedVertices[faceId * interleavedVertices.size() / 6];
    }
    else
    {
        // invalid ID, return the beginging of array
        return interleavedVertices.data();
    }
}








// static functions ===========================================================
///////////////////////////////////////////////////////////////////////////////
// return face normal (4th param) of a triangle v1-v2-v3
// if a triangle has no surface (normal length = 0), then return a zero vector
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float n[3])
{
    const float EPSILON = 0.000001f;

    // default return value (0, 0, 0)
    n[0] = n[1] = n[2] = 0;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = v2[0] - v1[0];
    float ey1 = v2[1] - v1[1];
    float ez1 = v2[2] - v1[2];
    float ex2 = v3[0] - v1[0];
    float ey2 = v3[1] - v1[1];
    float ez2 = v3[2] - v1[2];

    // cross product: e1 x e2
    float nx, ny, nz;
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if(length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        n[0] = nx * lengthInv;
        n[1] = ny * lengthInv;
        n[2] = nz * lengthInv;
    }
}



///////////////////////////////////////////////////////////////////////////////
// get the scale factor for vector to resize to the given length of vector
///////////////////////////////////////////////////////////////////////////////
float Cubesphere::computeScaleForLength(const float v[3], float length)
{
    // and normalize the vector then re-scale to new radius
    return length / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}



///////////////////////////////////////////////////////////////////////////////
// generate vertices for +X face only by intersecting 2 circular planes
// (longitudinal and latitudinal) at the longitude/latitude angles
///////////////////////////////////////////////////////////////////////////////
std::vector<float> Cubesphere::getUnitPositiveX(unsigned int pointsPerRow)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along y-axis
    float a2;           // latitudinal angle
    float scale;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            scale = Cubesphere::computeScaleForLength(v, 1);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);

            // DEBUG
            //std::cout << "vertex: (" << v[0] << ", " << v[1] << ", " << v[2] << "), "
            //          << sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) << std::endl;
        }
    }

    return vertices;
}

///////////////////////////////////////////////////////////////////////////////
// rescale vertex length
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::scaleVertex(float v[3], float scale)
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}












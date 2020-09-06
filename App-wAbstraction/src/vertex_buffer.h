#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include<GL\glew.h>

class VertexBuffer
{
    private:
        unsigned int m_RendererID;

    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void bind() const;
        void unbind() const;
};

#endif
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include<GL/glew.h>

class VertexBuffer
{
    private:
        unsigned int m_RendererID;

    public:
        VertexBuffer() {}
        ~VertexBuffer() {}

        void allocate(const void* data, unsigned int size);
        void bind() const;
        void unbind() const;
        void destroy();
        inline unsigned int getID() const { return m_RendererID; } 
};

#endif
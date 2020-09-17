#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray
{
    private:
        unsigned int m_RendererID;

    public:
        VertexArray() {}
        ~VertexArray() {}
        void create();
        void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
        void bind() const;
        void unbind() const;
        void destroy() const;
        inline unsigned int getID() const { return m_RendererID; } 

};

#endif
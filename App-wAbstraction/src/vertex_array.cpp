#include "vertex_array.h"

// VertexArray::VertexArray()
// {
//     glGenVertexArrays(1, &m_RendererID);
// }

VertexArray::~VertexArray()
{
    //glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::create(){
    glGenVertexArrays(1, &m_RendererID);
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    bind();
    vb.bind();
    const std::vector<VertexBufferElement> elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const VertexBufferElement element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized,
                              layout.GetStride(), (void *)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
};


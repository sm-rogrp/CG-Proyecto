#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

class IndexBuffer
{
    private:
        unsigned int m_RendererID;
        unsigned int m_Count;

    public:
        IndexBuffer() {}
        // IndexBuffer(const unsigned int* indices, unsigned int count);
        ~IndexBuffer();
        void setData(const unsigned int* indices, unsigned int count);
        void bind() const;
        void unbind() const;

        inline unsigned int getCount() const { return m_Count; }
};

#endif

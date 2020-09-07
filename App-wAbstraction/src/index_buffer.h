#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

class IndexBuffer
{
    private:
        unsigned int m_RendererID;
        unsigned int m_Count;

    public:
        IndexBuffer() {}
        ~IndexBuffer() {}

        void allocate(const unsigned int* indices, unsigned int count);
        void bind() const;
        void unbind() const;
        void destroy() const;
        
        inline unsigned int getID() const { return m_RendererID; }
        inline unsigned int getCount() const { return m_Count; }
};

#endif
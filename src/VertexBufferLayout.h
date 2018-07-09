//
// Created by strongheart on 7/8/18.
//

#ifndef PARTICLE_ATTRACTOR_OPENGL_VERTEXBUFFERLAYOUT_H
#define PARTICLE_ATTRACTOR_OPENGL_VERTEXBUFFERLAYOUT_H

#include <GL/glew.h>
#include <vector>
#include <cstdlib>


struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    unsigned int offset;
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
public:
    VertexBufferLayout() = default;

    template<typename T>
    void Push(unsigned int count,  unsigned int offset)
    {
        static_assert(true, "Error should not reach this statement");
    }

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
};

template<> inline
void VertexBufferLayout::Push<float>(unsigned int count, unsigned int offset)
{
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE, offset});
}

template<> inline
void VertexBufferLayout::Push<unsigned int>(unsigned int count, unsigned int offset)
{
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, offset});
}

template<> inline
void VertexBufferLayout::Push<unsigned char>(unsigned int count, unsigned int offset)
{
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, offset});
}

#endif //PARTICLE_ATTRACTOR_OPENGL_VERTEXBUFFERLAYOUT_H

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

    static unsigned int getSizeOfType(unsigned int type) {
        switch(type) {
            case GL_FLOAT:              return sizeof(GLfloat);
            case GL_UNSIGNED_INT:       return sizeof(GLuint);
            case GL_UNSIGNED_BYTE:      return sizeof(GLubyte);
        }
        static_assert(true, "Error should not reach this statement");
        return 0;
    }
};

class VertexBufferLayout
{
private:
    unsigned int stride;
    std::vector<VertexBufferElement> m_Elements;
public:
    VertexBufferLayout(): stride(0) {}

    template<typename T>
    void Push(unsigned int count)
    {
        static_assert(true, "Error should not reach this statement");
    }

    inline const unsigned int getStride() const { return stride; }
    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
};

template<> inline
void VertexBufferLayout::Push<float>(unsigned int count)
{
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE});
    stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template<> inline
void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE});
    stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template<> inline
void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE});
    stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}

#endif //PARTICLE_ATTRACTOR_OPENGL_VERTEXBUFFERLAYOUT_H

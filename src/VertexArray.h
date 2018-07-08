//
// Created by strongheart on 7/8/18.
//

#ifndef PARTICLE_ATTRACTOR_OPENGL_VERTEXARRAY_H
#define PARTICLE_ATTRACTOR_OPENGL_VERTEXARRAY_H

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};


#endif //PARTICLE_ATTRACTOR_OPENGL_VERTEXARRAY_H

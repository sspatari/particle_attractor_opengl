//
// Created by strongheart on 7/8/18.
//

#ifndef PARTICLE_ATTRACTOR_OPENGL_VERTEXBUFFER_H
#define PARTICLE_ATTRACTOR_OPENGL_VERTEXBUFFER_H

class VertexBuffer
{
private:
    unsigned int m_RendererID;

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};


#endif //PARTICLE_ATTRACTOR_OPENGL_VERTEXBUFFER_H

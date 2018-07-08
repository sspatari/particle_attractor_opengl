//
// Created by strongheart on 7/8/18.
//

#ifndef PARTICLE_ATTRACTOR_OPENGL_RENDERER_H
#define PARTICLE_ATTRACTOR_OPENGL_RENDERER_H


#include "shaders/Shader.h"
#include "VertexArray.h"

class Renderer
{
private:
    Shader shader;
    VertexArray va;

public:
    Renderer();
    void render();
    void cleanUp();

private:
    void prepare();

};


#endif //PARTICLE_ATTRACTOR_OPENGL_RENDERER_H

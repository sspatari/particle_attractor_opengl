//
// Created by strongheart on 7/8/18.
//

#ifndef PARTICLE_ATTRACTOR_OPENGL_RENDERER_H
#define PARTICLE_ATTRACTOR_OPENGL_RENDERER_H

#include "CudaGlobal.h"
#include "shaders/Shader.h"
#include "VertexArray.h"

class Renderer
{
private:
    Shader shader;
    VertexArray va;
    float *h_data;  // contain positions and colors on host
    float *h_state; // contain vec3 force and mass on host
    void *d_state; // contain vec3 force and mass memory on device
    cudaGraphicsResource *cuda_vbo_resource = nullptr;

    static int const MESH_WIDTH = 2;
    static int const MESH_HEIGHT = 2;

    void createBuffers();
public:
    Renderer();
    void render();
    void cleanUp();

private:
    void prepare();

};


#endif //PARTICLE_ATTRACTOR_OPENGL_RENDERER_H

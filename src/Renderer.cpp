//
// Created by strongheart on 7/8/18.
//

#include <GL/glew.h>
#include <iostream>
#include "Renderer.h"

Renderer::Renderer() : shader("src/shaders/particle.glsl")
{
    // additive blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_PROGRAM_POINT_SIZE);

//    float positions[] = {
//        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.5f, // 0
//         0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.5f, // 1
//         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.5f, // 2
//        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.5f, // 3
//    };
//
//    VertexBuffer vb(positions, 2 * 4 * 4 * sizeof(float));
//
//    VertexBufferLayout layout;
//    layout.Push<float>(4);
//    layout.Push<float>(4);
//    va.AddBuffer(vb, layout);
//
//    shader.Bind();
//
//    va.Unbind();
//    vb.Unbind();
//    shader.Unbind();
    //create buffers related to OpenGL and CUDA;
    createBuffers();
}

void Renderer::createBuffers()
{
    //allocate memory on cpu
    unsigned int MESH_COUNT = MESH_WIDTH * MESH_HEIGHT;
    h_data  = new float[MESH_COUNT * 8];

    // initializing vertex postions and color
    for(unsigned int i=0; i<MESH_COUNT*8; i+=8)
    {
        h_data[i]   = 0.0f;  // vertex.x
        h_data[i+1] = 0.0f;  // vertex.y
        h_data[i+2] = 0.0f;  // vertex.z
        h_data[i+3] = 1.0f;  //vertex.w

        // color is set in the kernel code but for debug purposes we'll set it
        // to white here
        h_data[i+4] = 1.0f;     // Red
        h_data[i+5] = 1.0f;   // Green
        h_data[i+6] = 1.0f;   // Blue
        h_data[i+7] = 0.05f;  // Alpha
    }

    VertexBuffer vb(h_data,  MESH_COUNT * 8 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(4);
    layout.Push<float>(4);
    va.AddBuffer(vb, layout);

    va.Unbind();
    vb.Unbind();

    /* CUDA: register OpenGL buffer with CUDA*/
    CHECK_CUDA(cudaGraphicsGLRegisterBuffer(&cuda_vbo_resource,
                                            vb.getBufferId(),
                                            cudaGraphicsMapFlagsNone));

    /* each particle has its own state
      with a force(vec3) and particles mass value */
    h_state = new float[MESH_COUNT*4];

    for(unsigned int i=0; i<MESH_COUNT*4; i+=4)
    {
        h_state[i]   = 0.0f;  // force.x
        h_state[i+1] = 0.0f;  // force.y
        h_state[i+2] = 0.0f;  // force.z
        // random mass of particle
        h_state[i+3] = 75.0f + (static_cast<float>(rand())/RAND_MAX) * 25.0f;
    }

    // malloc for cuda: cudaMalloc(devicePtr,size)
    CHECK_CUDA(cudaMalloc(&d_state,
                          MESH_COUNT * 4 * sizeof(float)));
    // transfer host data to gpu: cudaMemcpy(devicePtr, hostPtr, size, kind)
    CHECK_CUDA(cudaMemcpy(d_state,
                          static_cast<const void *>(h_state),
                          MESH_COUNT * 4 * sizeof(float),
                          cudaMemcpyHostToDevice));

    /* We won't need this stuff again on the host system */
    delete[] h_data;
    delete[] h_state;
    h_data = nullptr;
    h_state = nullptr;
}

void Renderer::render()
{
    prepare();
    shader.Bind();
    va.Bind();

    glDrawArrays(GL_POINTS, 0, MESH_WIDTH * MESH_HEIGHT);
}

void Renderer::cleanUp()
{
    // unregister this buffer object with CUDA
    cudaGraphicsUnregisterResource(cuda_vbo_resource);
    CHECK_CUDA( cudaFree(d_state) );
    d_state = nullptr;

    shader.Unbind();
    va.Unbind();
}

void Renderer::prepare()
{
    // Set the clear color
    glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
}

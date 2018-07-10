//
// Created by strongheart on 7/8/18.
//

#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"
#include "DisplayManager.h"
#include "kernel.h"

Renderer::Renderer() : shader("src/shaders/particle.glsl")
{
    // additive blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //set the GPU to use
    CHECK_CUDA(cudaSetDevice(0));

    //get the number of procesors that the device has
    int value;
    cudaDeviceGetAttribute(&value, cudaDevAttrMultiProcessorCount, 0);
    numSMs = static_cast<unsigned int>(value);

    //create buffers related to OpenGL and CUDA;
    createBuffers();
    lastTime = DisplayManager::getCurrentTime();
}

void Renderer::createBuffers()
{
    //allocate memory on cpu
    unsigned int MESH_COUNT = MESH_WIDTH * MESH_HEIGHT;
    h_data  = new float[MESH_COUNT * 8];

    // initializing vertex postions and color
    for(unsigned int i=0; i<MESH_COUNT*4; i+=4) {
        h_data[i] = 0.0f;  // vertex.x
        h_data[i + 1] = 0.0f;  // vertex.y
        h_data[i + 2] = 0.0f;  // vertex.z
        h_data[i + 3] = 1.0f;  // vertex.w
    }

    for(unsigned int i=MESH_COUNT*4; i<MESH_COUNT*8; i+=4)
    {
        // color is set in the kernel code but for debug purposes we'll set it
        // to white here
        h_data[i]     = 1.0f;   // Red
        h_data[i + 1] = 1.0f;   // Green
        h_data[i + 2] = 1.0f;   // Blue
        h_data[i + 3] = 0.05f;   // Alpha
    }

    VertexBuffer vb(h_data,  MESH_COUNT * 8 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(4, 0);
    layout.Push<float>(4, MESH_COUNT * 4 * sizeof(float));
    va.AddBuffer(vb, layout);

    va.Unbind();
    vb.Unbind();

    /* CUDA: register OpenGL buffer object for access CUDA*/
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
        h_state[i+3] = 75.0f + static_cast<float>(rand()) / RAND_MAX * 25.0f;
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

void Renderer::runCuda(double delta)
{
    float4 *dptr;
    size_t num_bytes;
    // map OpenGL buffer object to CUDA
    // Note: OpenGL binding and CUDA mapping on a buffer are mutually exclusive
    //  you will not be able to map a buffer if it is bound by OpenGL
    //
    // DEPRECATED: cudaGLMapBufferObject((void**)&dptr, vbo);
    CHECK_CUDA(cudaGraphicsMapResources(1, &cuda_vbo_resource, 0));
    CHECK_CUDA(cudaGraphicsResourceGetMappedPointer((void **) &dptr,
                                                    &num_bytes,
                                                    cuda_vbo_resource));

    if(DisplayManager::framebufferSizeWasReset()) {
        kernel_reset( dptr, static_cast<float4*>(d_state),
                      DisplayManager::getFrameWidth(), DisplayManager::getFrameHeight(),
                      MESH_WIDTH, MESH_HEIGHT,
                      numSMs );
        DisplayManager::resetFrameBufferResetFlag();
    }else{
        // this returns cursor pos in screen coordinates (maybe you have to convert to frame size coordinates)
        DisplayManager::CursorPosition curentCursorPos = DisplayManager::getCursorPos();
        auto msx = static_cast<float>(curentCursorPos.x);
        auto msy = (DisplayManager::getFrameHeight() - static_cast<float>(curentCursorPos.y));

        kernel_advance(dptr, static_cast<float4 *>(d_state),
                       msx, msy,
                       MESH_WIDTH * MESH_HEIGHT,
                       speed, numSMs, delta);
    }


    CHECK_LAST("Kernel launch failed.");
    // unmap buffer object
    // DEPRECATED: checkCudaErrors(cudaGLUnmapBufferObject(vbo));
    CHECK_CUDA(cudaGraphicsUnmapResources(1, &cuda_vbo_resource, 0));
}

void Renderer::render()
{
    prepare();
    shader.Bind();
    va.Bind();

    currentTime = DisplayManager::getCurrentTime();
    double delta = 100.0*(currentTime-lastTime);
    lastTime = currentTime;

    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection = glm::ortho( 0.0f, 1.0f * DisplayManager::getFrameWidth(),
                                       0.0f, 1.0f * DisplayManager::getFrameHeight(),
                                       1.f, -1.f );

    shader.SetUniformMat4f("u_model", model);
    shader.SetUniformMat4f("u_view", view);
    shader.SetUniformMat4f("u_projection", projection);

    runCuda(delta);

    glDrawArrays(GL_POINTS, 0, MESH_WIDTH * MESH_HEIGHT);
}

void Renderer::cleanUp()
{
    // unregister this buffer object with CUDA
    cudaGraphicsUnregisterResource(cuda_vbo_resource);
    CHECK_CUDA(cudaFree(d_state));
    d_state = nullptr;

    shader.Unbind();
    va.Unbind();
}

void Renderer::prepare()
{

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
}

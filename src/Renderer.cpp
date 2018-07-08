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

    float value[2];
    float step;
    float size;
    glGetFloatv(GL_POINT_SIZE_RANGE, value);
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
    glGetFloatv(GL_POINT_SIZE, &size);

    std::cout << "range: " << value[0] << " " << value[1] << std::endl;
    std::cout << "step = " << step << std::endl;
    std::cout << "size = " << size << std::endl;


    float positions[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, // 0
        0.5f, -0.5f, 0.0f, 1.0f, // 1
        0.5f,  0.5f, 0.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f, 1.0f, // 3
        1.0f,  0.0f, 0.0f, 0.5f,  // 0(color)
        1.0f,  0.0f, 0.0f, 0.5f,  // 1(color)
        1.0f,  0.0f, 0.0f, 0.5f,  // 2(color)
        1.0f,  0.0f, 0.0f, 0.5f,  // 3(color)
    };

    VertexBuffer vb(positions, 2 * 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(4, 0);
    layout.Push<float>(4, 4 * 4 * sizeof(float));
    va.AddBuffer(vb, layout);

    shader.Bind();

    va.Unbind();
    vb.Unbind();
    shader.Unbind();
}

void Renderer::render()
{
    prepare();
    shader.Bind();
    va.Bind();

    glDrawArrays(GL_POINTS, 0, 2 * 4 * 4);
}

void Renderer::cleanUp()
{
    shader.Unbind();
}

void Renderer::prepare()
{
    // Set the clear color
    glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
}

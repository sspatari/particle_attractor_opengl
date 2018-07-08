#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shaders/Shader.h"
#include "VertexArray.h"
#include "DisplayManager.h"

int frameWidth;
int frameHeight;



int main()
{
    DisplayManager::createDisplay();

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

    VertexArray va;
    VertexBuffer vb(positions, 2 * 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(4, 0);
    layout.Push<float>(4, 4 * 4 * sizeof(float));
    va.AddBuffer(vb, layout);

    Shader shader("src/shaders/particle.glsl");
    shader.Bind();

    va.Unbind();
    vb.Unbind();
    shader.Unbind();

    /* Loop until the user closes the window */
    while (!DisplayManager::isCloseRequested())
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        va.Bind();

        glDrawArrays(GL_POINTS, 0, 2 * 4 * 4);

        DisplayManager::updateDisplay();
    }

    DisplayManager::closeDisplay();
    return 0;
}
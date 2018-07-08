#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shaders/Shader.h"
#include "VertexArray.h"

int frameWidth;
int frameHeight;

static void APIENTRY openglErrorCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam) {
    (void)source; (void)type; (void)id;
    (void)severity; (void)length; (void)userParam;
    std::cerr << "[Opengl Error] (" << id << "): "<< message << std::endl;
    if (severity==GL_DEBUG_SEVERITY_HIGH) {
        std::cerr << "Aborting..." << std::endl;
        abort();
    }
}

static void errorCallback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    frameWidth = width;
    frameHeight = height;
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}

int main()
{
    GLFWwindow* window;

    /* Set GLFW error callback */
    glfwSetErrorCallback(errorCallback);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Particle attractor", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to open GLFW window. Probably a GPU and OpenGL driver issue." << std::endl;
        glfwTerminate();
        return -1;
    }

    /* doc: On some machines screen coordinates and pixels are the same, but on others they will not be.
     * So we get the real frame size in pixels here */
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    /* Set the callbacks */
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Enable v-sync */
    glfwSwapInterval(1);

    /* Initiate Glew that heps in quering and loading opengl extensions */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }

    /* Output version of OpenGL that is used*/
    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Enable the opengl's debug callback */
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglErrorCallback, nullptr);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, static_cast<GLboolean>(true)
    );

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
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        va.Bind();

        glDrawArrays(GL_POINTS, 0, 2 * 4 * 4);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
//
// Created by strongheart on 7/8/18.
//

#include <iostream>
#include "DisplayManager.h"

int DisplayManager::frameWidth = 0;
int DisplayManager::frameHeight = 0;
GLFWwindow* DisplayManager::window = nullptr;

void APIENTRY DisplayManager::openglErrorCallback(
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

void DisplayManager::errorCallback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

void DisplayManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void DisplayManager::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    frameWidth = width;
    frameHeight = height;
}

void DisplayManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void DisplayManager::createDisplay()
{

    /* Set GLFW error callback */
    glfwSetErrorCallback(errorCallback);

    /* Initialize the library */
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Particle attractor", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to open GLFW window. Probably a GPU and OpenGL driver issue." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
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
}

void DisplayManager::closeDisplay()
{
    glfwTerminate();
}

void DisplayManager::updateDisplay()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}

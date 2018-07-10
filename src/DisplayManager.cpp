//
// Created by strongheart on 7/8/18.
//

#include <iostream>
#include "DisplayManager.h"

bool DisplayManager::resetFlag = 1;
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
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high";
                                             std::cerr << "Aborting..." << std::endl;
                                             abort();
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
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
    resetFlag = 1;
    frameWidth = width;
    frameHeight = height;
}

void DisplayManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}

double DisplayManager::getCurrentTime()
{
    return glfwGetTime();
}

DisplayManager::CursorPosition DisplayManager::getCursorPos()
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return { x, y };
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
    // Tells OpenGL to directly call the callback function the moment an error occurred.
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

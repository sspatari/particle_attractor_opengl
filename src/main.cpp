#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
    std::cerr << "Opengl Error: " << message << std::endl;
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


    int frameWidth;
    int frameHeight;

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

    /* Enable the debug callback */
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglErrorCallback, nullptr);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, static_cast<GLboolean>(true)
    );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
//
// Created by strongheart on 7/8/18.
//

#ifndef PARTICLE_ATTRACTOR_OPENGL_DISPLAYMANAGER_H
#define PARTICLE_ATTRACTOR_OPENGL_DISPLAYMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class DisplayManager
{
private:
    static int const WIDTH = 640;
    static int const HEIGHT = 480;
    static int frameWidth, frameHeight;
    static GLFWwindow* window;

    DisplayManager() = default;
    ~DisplayManager() = default;
    static void APIENTRY openglErrorCallback(GLenum source,
                                             GLenum type,
                                             GLuint id,
                                             GLenum severity,
                                             GLsizei length,
                                             const GLchar* message,
                                             const void* userParam);
    static void errorCallback(int error, const char* description);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
    static void createDisplay();
    static void closeDisplay();
    inline static int isCloseRequested() { return glfwWindowShouldClose(window); }
    static void updateDisplay();
    inline static GLFWwindow* getWindow() { return  window;};
};


#endif //PARTICLE_ATTRACTOR_OPENGL_DISPLAYMANAGER_H

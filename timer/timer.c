#include "timer.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>

#include "maths/utils.h"

struct events events = { 0 };

// timer custom callbacks

void timerKeyCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods)
{
    UNUSED(scancode);
    if (events.key)
        events.key(key, action, mods, glfwGetWindowUserPointer(window));
}

void timerMouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    static double lastX = 0;
    static double lastY = 0;
    static int firstMouse = 1;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = 0;
    }
    double xOffset = xpos - lastX;
    double yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if (events.mouse)
        events.mouse(xOffset, yOffset, glfwGetWindowUserPointer(window));
}

void frameBuffersizeCallback(GLFWwindow *window, int width, int height)
{
    UNUSED(window);
    glViewport(0, 0, width, height);
}

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                   GLsizei length, const GLchar *message, const void *userParam)
{
    UNUSED(source);
    UNUSED(type);
    UNUSED(id);
    UNUSED(length);
    UNUSED(userParam);
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_MEDIUM:
        fprintf(stdout, "WARNING: %s \n", message);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        fprintf(stdout, "ERROR: %s \n", message);
        exit(-1);
        break;
    default:
        // fprintf(stdout, "INFO: %s \n", message);
        return;
    }
}

// timer functions

void registerEvents(void)
{
    glfwSetKeyCallback(glfwGetCurrentContext(), timerKeyCallback);
    glDebugMessageCallback(debugCallback, NULL);
    glfwSetFramebufferSizeCallback(glfwGetCurrentContext(),
                                   frameBuffersizeCallback);
    glfwSetCursorPosCallback(glfwGetCurrentContext(), timerMouseCallback);
}

void updateFrame(world *w, float deltaTime)
{
    glfwPollEvents();
    updateWorld(w);
    if (events.update)
        events.update(w, deltaTime);
}

void registerKeyHandler(keyHandler handler)
{
    events.key = handler;
}

void registerUpdateHandler(updateHandler handler)
{
    events.update = handler;
}
void registerMouseHandler(mouseHandler handler, bool lockCursor)
{
    if (lockCursor)
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR,
                         GLFW_CURSOR_NORMAL);
    events.mouse = handler;
}

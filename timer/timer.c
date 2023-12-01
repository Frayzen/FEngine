#include "timer.h"
#include "maths/utils.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>

struct events events = { 0 };

//timer custom callbacks

void timerKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    UNUSED(scancode);
    if (events.key)
        events.key(key, action, mods, glfwGetWindowUserPointer(window));
}

void frameBuffersizeCallback(GLFWwindow* window, int width, int height)
{
    UNUSED(window);
    glViewport(0, 0, width, height);
}


void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    UNUSED(source);
    UNUSED(type);
    UNUSED(id);
    UNUSED(length);
    UNUSED(userParam);
    switch (severity) {
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
    glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), frameBuffersizeCallback);
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

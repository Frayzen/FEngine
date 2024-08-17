#include "tools.hh"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *msg, const void *userParam) {
    (void)source;
    (void)type;
    (void)id;
    (void)severity;
    (void)length;
    (void)userParam;
    std::cout << msg << '\n';
    std::flush(std::cout);
}


void cleanExit(int code) {
    glfwTerminate();
    if (glfwGetCurrentContext() != nullptr)
        glfwDestroyWindow(glfwGetCurrentContext());
    exit(code);
}
void checkGLError(const char *message) {
    GLenum error = glGetError();
    FAIL_ON(error != GL_NO_ERROR,
            "OpenGL Error (" << message << "): " << glewGetErrorString(error));
}

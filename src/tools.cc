#include "tools.hh"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

void clean_exit(int code) {
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

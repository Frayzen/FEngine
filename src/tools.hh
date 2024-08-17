#pragma once

#include <cstdlib>
#include <glm/fwd.hpp>
#include <GL/glew.h>
#include <iostream>

// Macros
#define FAIL_ON(Condition, Message)                                            \
    if (Condition) {                                                           \
        std::cerr << Message << std::endl;                                     \
        cleanExit(-1);                                                        \
    }


void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *msg, const void *userParam);
void cleanExit(int code);
void checkGLError(const char *message);

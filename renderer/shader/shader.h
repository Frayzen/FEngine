#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLint createShader(GLenum shaderType, const char *path);

#endif /* !SHADER_H */

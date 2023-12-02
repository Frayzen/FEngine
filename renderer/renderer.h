#ifndef RENDERER_H
#define RENDERER_H

#include "renderer/world/mesh/mesh.h"
#include "renderer/world/world.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct renderer{
    GLFWwindow *window;
    GLint vertexShader;
    GLint fragmentShader;
    GLint shaderProgram;
    graphic **graphics;
    unsigned int graphicsCount;
} renderer;

#define GET_RENDERER glfwGetWindowUserPointer(glfwGetCurrentContext())

renderer *initRenderer(void);
int startRendering(world *w);
void destroyRenderer(void);

void defineVertexShader(const char *path);
void defineFragmentShader(const char *path);

unsigned int addGraphic(graphic *gc);

#endif /* !RENDERER_H */

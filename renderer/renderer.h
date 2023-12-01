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
} renderer;

renderer *initRenderer(void);
void defineVertexShader(renderer *rd, const char *path);
void defineFragmentShader(renderer *rd, const char *path);
int startRendering(renderer *rd, world *w);
void destroyRenderer(renderer *rd);

#endif /* !RENDERER_H */

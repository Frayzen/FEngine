#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

struct Renderer{
    struct mesh* meshes; 
};

struct Renderer initialize(void);
void destroyRenderer(struct Renderer *rd);

#endif /* !RENDERER_H */

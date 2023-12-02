#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "renderer/world/mesh/mesh.h"
#include <GL/glew.h>
#include <GL/gl.h>

typedef struct graphic{
    GLuint VBO;
    GLuint VNBO;
    GLuint VAO;
    GLuint EBO;
} graphic;

graphic *createMeshGraphic(mesh *m);

#endif /* !GRAPHIC_H */

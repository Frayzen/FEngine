#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "renderer/world/mesh/mesh.h"
#include <GL/glew.h>
#include <GL/gl.h>

typedef struct graphic{
    GLuint *textArray;
    GLuint VBO;
    GLuint VTBO;
    GLuint VNBO;
    GLuint VAO;
    GLuint EBO;
} graphic;

graphic *createMeshGraphic(mesh *m);
void destroyGraphic(graphic *g);

#endif /* !GRAPHIC_H */

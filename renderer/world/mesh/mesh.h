#ifndef MESH_H
#define MESH_H

#include "maths/matrix.h"
#include "maths/utils.h"
#define _POSIX_C_SOURCE 200809L
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdio.h>

typedef struct mesh_graphic{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
} mesh_graphic;

typedef struct mesh{
    transform transform;
    //vector data
    float*v;
    unsigned int v_count;
    float *vt;
    unsigned int vt_count;
    //triangles
    unsigned int *v_ids;
    unsigned int *vt_ids;
    unsigned int tris_count;
    //graphic
    mesh_graphic graphic;
} mesh;

typedef void (*lineParser)(char *line, size_t size, mesh *m);

mesh *createMeshFromObj(const char *path);
void createMeshGraphic(mesh *m);
void destroyMesh(mesh *mesh);
void printMesh(mesh* m);

#endif /* !MESH_H */

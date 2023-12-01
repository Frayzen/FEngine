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
    vec3*v;
    unsigned int v_count;
    vec2 *vt;
    unsigned int vt_count;
    //triangles
    vec3i *v_ids;
    vec2i *vt_ids;
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

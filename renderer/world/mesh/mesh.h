#ifndef MESH_H
#define MESH_H

#include "maths/matrix.h"
#include "maths/utils.h"
#include <stdbool.h>
#define _POSIX_C_SOURCE 200809L
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdio.h>

typedef struct vect
{
    vec3 pos;
    vec2 uv;
    vec3 normal;
} vect;

typedef struct mesh{
    ssize_t meshID;
    //vector data
    vect *v;
    unsigned int v_count;
    //triangles
    vec3i *v_ids;
    unsigned int tris_count;
    const char **texturesPath;
} mesh;


mesh *createMesh(void);
void destroyMesh(mesh *mesh);
void printMesh(mesh* m, bool showVertices);
void addTexture(mesh *m, const char *path);

#endif /* !MESH_H */

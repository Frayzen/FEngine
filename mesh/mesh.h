#ifndef MESH_H
#define MESH_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

typedef struct tri{
    int v_ids[3];
    int vt_ids[3];
} tri;

typedef struct mesh{
    float*v;
    unsigned int v_nb;
    float *vt;
    unsigned int vt_nb;
    tri* tris;
    unsigned int tris_nb;
} mesh;

typedef void (*lineParser)(char *line, size_t size, mesh *m);

mesh*create_mesh_from_obj(const char *path);
void destroy_mesh(mesh *mesh);
void print_mesh(mesh* m);

#endif /* !MESH_H */

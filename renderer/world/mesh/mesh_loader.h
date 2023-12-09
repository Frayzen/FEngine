#ifndef MESH_LOADR_H
#define MESH_LOADR_H

#include "maths/vector.h"
#include "mesh.h"
#include <stdio.h>

typedef struct mesh_builder
{
    vec3 *v;
    vec2 *vt;
    vec3 *vn;
    vec3i *tris_v;
    vec3i *tris_vt;
    vec3i *tris_vn;
    size_t v_count;
    size_t vt_count;
    size_t vn_count;
    size_t tris_count;
} meshBuilder;

typedef void (*lineParser)(char *line, size_t size, meshBuilder *m);

mesh *createMeshFromObj(const char *path);

#endif /* !MESH_LOADR_H */

#include "mesh.h"
#include "renderer/renderer.h"
#include <stdio.h>
#include <stdlib.h>

mesh *createMesh(void)
{
    mesh *m = calloc(sizeof(mesh), 1);
    m->meshID = -1;
    return m;
}

void destroyMesh(mesh *mesh)
{
    if (!mesh)
        return;
    free(mesh->v);
    free(mesh->vt);
    free(mesh->v_ids);
    free(mesh->vt_ids);
    free(mesh);
}

void printMesh(mesh* m)
{
    printf("NBV %d\n", m->v_count);
    printf("NBVT %d\n", m->vt_count);
    printf("NBTRI %d\n", m->tris_count);
    printf("====\n");
    for (size_t i = 0; i < m->v_count; i++) {
        printf("%f %f %f\n", m->v[i].x, m->v[i].y, m->v[i].z);
    }
    printf("====\n");
    for (size_t i = 0; i < m->tris_count; i++) {
        printf("%d\n", m->v_ids[i].x);
        printf("%d\n", m->v_ids[i].y);
        printf("%d\n", m->v_ids[i].z);
        printf("====\n");
    }
}

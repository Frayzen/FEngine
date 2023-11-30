#include "mesh.h"
#include <stdlib.h>

void destroy_mesh(struct mesh *mesh)
{
    if (!mesh)
        return;
    free(mesh->v);
    free(mesh->vt);
    free(mesh->tris);
    free(mesh);
}

void print_mesh(struct mesh* m)
{
    printf("NBV %d\n", m->v_nb);
    printf("NBVT %d\n", m->vt_nb);
    printf("NBTRI %d\n", m->tris_nb);
    printf("====\n");
    for (size_t i = 0; i < 15; i++) {
        printf("%d\n", m->tris[i].v_ids[0]);
        printf("%d\n", m->tris[i].v_ids[1]);
        printf("%d\n", m->tris[i].v_ids[2]);
        printf("----\n");
        printf("%d\n", m->tris[i].vt_ids[0]);
        printf("%d\n", m->tris[i].vt_ids[1]);
        printf("%d\n", m->tris[i].vt_ids[2]);
        printf("====\n");
    }
}

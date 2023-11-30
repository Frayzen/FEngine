#include "mesh.h"
#include <stdlib.h>

void destroy_mesh(struct mesh *mesh)
{
    free(mesh->v);
    free(mesh->vt);
    free(mesh->tris);
    free(mesh);
}

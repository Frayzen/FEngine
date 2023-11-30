#include "mesh/mesh.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;
    struct mesh *m = create_mesh_from_obj(argv[1]);
    destroy_mesh(m);
    return 0;
}

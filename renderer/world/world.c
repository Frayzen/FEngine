#include "world.h"
#include <stdlib.h>

world *createWorld(void)
{
    world *w = calloc(1, sizeof(world));
    return w;
}
void destroyWorld(world *w)
{
    if (w->meshes)
    {
        for (unsigned int i = 0; i < w->mesh_count; i++)
            destroyMesh(w->meshes[i]);
        free(w->meshes);
    }
    if (w->cam)
    {
        for (unsigned int i = 0; i < w->cam_count; i++)
            destroyCamera(w->cam[i]);
        free(w->cam);
    }
    free(w);
}
void addCamera(world *w, camera *cam)
{
    w->cam = realloc(w->cam, sizeof(camera *) * (w->cam_count + 1));
    w->cam[w->cam_count] = cam;
    w->cam_count++;
}
void addMesh(world *w, mesh *m)
{
    createMeshGraphic(m); 
    w->meshes = realloc(w->meshes, sizeof(mesh *) * (w->mesh_count + 1));
    w->meshes[w->mesh_count] = m;
    w->mesh_count++;
}
void updateWorld(world *w)
{
    for (unsigned int i = 0; i < w->cam_count; i++)
        updateCamera(w->cam[i]);
}

#ifndef WORLD_H
#define WORLD_H

#include "renderer/world/camera/camera.h"
#include "renderer/world/mesh/mesh.h"
typedef struct world{
    unsigned int cur_cam;
    camera **cam;
    unsigned int cam_count;
    mesh **meshes;
    unsigned int mesh_count;
} world;

#define CURRENT_CAMERA(W) (W->cam[W->cur_cam])

world *createWorld(void);
void destroyWorld(world *w);
void addCamera(world *w, camera *cam);
void addMesh(world *w, mesh *m);
void getCamera(world *w);
void updateWorld(world *w);

#endif /* !WORLD_H */

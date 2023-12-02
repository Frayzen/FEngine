#ifndef WORLD_H
#define WORLD_H

#include "renderer/graphic/graphic.h"
#include "renderer/world/camera/camera.h"
#include "renderer/world/mesh/mesh.h"
#include "renderer/world/object/object.h"

#define MAX_OBJ_COUNT 500000

typedef struct world{
    unsigned int curCam;
    camera **cam;
    unsigned int camCount;
    object **objects;
    unsigned int objectsCount;
} world;

#define CURRENT_CAMERA(W) (W->cam[W->curCam])

world *createWorld(void);
void destroyWorld(world *w);
void getCamera(world *w);
void updateWorld(world *w);

object *addMesh(world *w, mesh *m);

void addCamera(world *w, camera *cam);

#endif /* !WORLD_H */

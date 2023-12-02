#include "world.h"
#include "maths/vector.h"
#include "renderer/graphic/graphic.h"
#include "renderer/renderer.h"
#include "renderer/world/object/object.h"
#include <stdlib.h>

world *createWorld(void)
{
    world *w = calloc(1, sizeof(world));
    return w;
}
void destroyWorld(world *w)
{
    if (w->objects)
    {
        for (unsigned int i = 0; i < w->objectsCount; i++)
        {
            switch (w->objects[i]->type) {
                case MESH:
                    destroyMesh(w->objects[i]->data.mesh);
                default:
                    return;
            }
        }
        free(w->objects);
    }
    if (w->cam)
    {
        for (unsigned int i = 0; i < w->camCount; i++)
            destroyCamera(w->cam[i]);
        free(w->cam);
    }
    free(w);
}
void addCamera(world *w, camera *cam)
{
    w->cam = realloc(w->cam, sizeof(camera *) * (w->camCount + 1));
    w->cam[w->camCount] = cam;
    w->camCount++;
}

void addObject(world *w, object *o)
{
    if (w->objectsCount == MAX_OBJ_COUNT)
        return;
    w->objectsCount++;
    w->objects = realloc(w->objects, w->objectsCount * sizeof(struct object *));
    w->objects[w->objectsCount - 1] = o;
}

object *addMesh(world *w, mesh *m)
{
    if (m->meshID == -1)
        m->meshID = addGraphic(createMeshGraphic(m));
    renderer *rd = GET_RENDERER;
    object *o = createObject(transformIdentity(), MESH, m, rd->graphics[m->meshID]);
    addObject(w, o);
    return o;
}

void updateWorld(world *w)
{
    for (unsigned int i = 0; i < w->camCount; i++)
        updateCamera(w->cam[i]);
}

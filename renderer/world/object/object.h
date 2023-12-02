#ifndef OBJECT_H
#define OBJECT_H

#include "maths/vector.h"
#include "renderer/graphic/graphic.h"
#include "renderer/world/mesh/mesh.h"

enum objectType{
    MESH,
};

union objectData{
    mesh *mesh;
    void *any;
};

typedef struct object{
    // link to graphic in renderer
    unsigned int objectID;
    transform transform;
    enum objectType type;
    union objectData data;
    graphic *graphic;
} object;

object *createObject(transform tf, enum objectType type, void *data, graphic *g);
graphic *createMeshGraphic(mesh *m);

#endif /* !OBJECT_H */

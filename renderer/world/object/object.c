#include "object.h"

#include <stdlib.h>

object *createObject(transform tf, enum objectType type, void *data, graphic *g)
{
    object *o = calloc(sizeof(object), 1);
    o->transform = tf;
    o->graphic = g;
    o->type = type;
    o->data.any = data;
    return o;
}

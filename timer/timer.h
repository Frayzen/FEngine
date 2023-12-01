#ifndef INPUTS_H
#define INPUTS_H

#include "renderer/world/world.h"
#include <GLFW/glfw3.h>

typedef void (*keyHandler)(int key, int action, int mods, world *w);
typedef void (*updateHandler)(world *w, float deltaTime);

struct events {
    updateHandler update;
    keyHandler key;
};

// deltaTime in seconds
void registerEvents(void);
void updateFrame(world *w, float deltaTime);

void registerKeyHandler(keyHandler handler);
void registerUpdateHandler(updateHandler handler);

#endif /* !INPUTS_H */

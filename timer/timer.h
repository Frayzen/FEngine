#ifndef INPUTS_H
#define INPUTS_H

#include "renderer/world/world.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef void (*keyHandler)(int key, int action, int mods, world *w);
typedef void (*updateHandler)(world *w, float deltaTime);
typedef void (*mouseHandler)(double xOffset, double yOffset, world *w);

struct events {
    updateHandler update;
    keyHandler key;
    mouseHandler mouse;
};

// deltaTime in seconds
void registerEvents(void);
void updateFrame(world *w, float deltaTime);

void registerKeyHandler(keyHandler handler);
void registerUpdateHandler(updateHandler handler);
void registerMouseHandler(mouseHandler handler, bool lockCursor);

#endif /* !INPUTS_H */

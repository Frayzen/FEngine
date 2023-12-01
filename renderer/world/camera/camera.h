#ifndef CAMERA_H
#define CAMERA_H

#include "maths/matrix.h"
#include "maths/utils.h"
typedef struct camera {
    transform transform;
    mat4 projection;
    mat4 view;
    float fov;
    float aspect;
    float near;
    float far;
} camera;

camera *createCamera(float fov, float aspect, float near, float far);
void destroyCamera(camera *cam);
void updateCamera(camera *cam);

#endif /* !CAMERA_H */

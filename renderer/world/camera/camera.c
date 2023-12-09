#include "camera.h"

#include <stdlib.h>

camera *createCamera(float fov, float aspect, float near, float far)
{
    camera *cam = malloc(sizeof(camera));
    cam->transform = transformIdentity();
    cam->projection = mat4Identity();
    cam->view = mat4Identity();
    cam->fov = fov;
    cam->aspect = aspect;
    cam->near = near;
    cam->far = far;
    return cam;
}

void destroyCamera(camera *cam)
{
    free(cam);
}

void updateCamera(camera *cam)
{
    cam->projection =
        mat4Perspective(cam->fov, cam->aspect, cam->near, cam->far);
    cam->view = mat4Transform(cam->transform);
}

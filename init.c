
#include "maths/matrix.h"
#include "maths/utils.h"
#include "maths/vector.h"
#include "renderer/world/mesh/mesh.h"
#include "renderer/renderer.h"
#include "renderer/world/world.h"
#include "timer/timer.h"
#include <math.h>

vec3 camVelocity = { 0 }; 
vec3 rotVelocity = { 0 };

void myKeyHandler(int key, int action, int mods, world *w)
{
    UNUSED(mods);
    UNUSED(w);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
    switch (key) {
        case GLFW_KEY_W:
            if (action == GLFW_PRESS)
                camVelocity.x = 1;
            else if (action == GLFW_RELEASE)
                camVelocity.x = 0;
            break;
        case GLFW_KEY_S:
            if (action == GLFW_PRESS)
                camVelocity.x = -1;
            else if (action == GLFW_RELEASE)
                camVelocity.x = 0;
            break;
        case GLFW_KEY_A:
            if (action == GLFW_PRESS)
                camVelocity.z = 1;
            else if (action == GLFW_RELEASE)
                camVelocity.z = 0;
            break;
        case GLFW_KEY_D:
            if (action == GLFW_PRESS)
                camVelocity.z = -1;
            else if (action == GLFW_RELEASE)
                camVelocity.z = 0;
            break;
        case GLFW_KEY_SPACE:
            if (action == GLFW_PRESS)
                camVelocity.y = -1;
            else if (action == GLFW_RELEASE)
                camVelocity.y = 0;
            break;
        case GLFW_KEY_LEFT_SHIFT:
            if (action == GLFW_PRESS)
                camVelocity.y = 1;
            else if (action == GLFW_RELEASE)
                camVelocity.y = 0;
            break;
    }
}

void myMouseHandler(double xOffset, double yOffset, world *w)
{
    UNUSED(w);
    rotVelocity.y -= xOffset;
    rotVelocity.z += yOffset;
}

void myUpdateHandler(world *w, float deltaTime)
{
    camera *cam = CURRENT_CAMERA(w);

    mat4 rotMat = mat4RotateY(mat4Identity(), cam->transform.rot.y);
    vec3 forward = mat4MulVec3(rotMat, VECTOR_FORWARD);
    vec3 right = mat4MulVec3(rotMat, VECTOR_RIGHT);
    vec3 up = mat4MulVec3(rotMat, VECTOR_UP);

    float movSpeed = 100;

    cam->transform.rot = vec3Add(cam->transform.rot, vec3Scale(rotVelocity, deltaTime * 100));
    cam->transform.pos = vec3Add(cam->transform.pos, vec3Scale(forward, camVelocity.x * deltaTime * movSpeed));
    cam->transform.pos = vec3Add(cam->transform.pos, vec3Scale(right, camVelocity.z * deltaTime * movSpeed));
    cam->transform.pos = vec3Add(cam->transform.pos, vec3Scale(up, camVelocity.y * deltaTime * movSpeed));

    rotVelocity = VEC3(0, 0, 0);
    w->meshes[0]->transform.rot.y += deltaTime;
}

int main(void)
{
    renderer *rd = initRenderer();
    defineVertexShader(rd, "./assets/vertexShader.glsl");
    defineFragmentShader(rd, "./assets/fragmentShadder.glsl");
    world *w = createWorld();
    camera *cam = createCamera(90, 16.0 / 9.0, 0.1, 100);
    addCamera(w, cam);
    mesh *m = createMeshFromObj("./assets/teddy.obj");
    m->transform.scale = VEC3(0.05, 0.05, 0.05);
    m->transform.pos = VEC3(-.5, -.5, 0);
    m->transform.rot = VEC3(M_PI / 6, 0, 0);
    addMesh(w, m);

    registerKeyHandler(myKeyHandler);
    registerUpdateHandler(myUpdateHandler);
    registerMouseHandler(myMouseHandler, true);

    // printMesh(m);
    
    startRendering(rd, w);

    destroyRenderer(rd);
    destroyWorld(w);
    return 0;
}

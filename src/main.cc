#include <algorithm>
#include <cstdlib>
#include <glm/fwd.hpp>
#include <iostream>
#include <GL/glew.h>
#include "constants.hh"
#include "mesh/mesh.hh"
#include "object/camera.hh"
#include "object/object.hh"
#include "shader/compute.hh"
#include "shader/render.hh"
#include "tools.hh"
#include <GLFW/glfw3.h>
#include <string.h>

void prettyPrintMat4(const glm::mat4 &matrix) {

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << matrix[i][j] << " ";
        }

        std::cout << std::endl;
    }
}
void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *msg, const void *userParam) {
    (void)source;
    (void)type;
    (void)id;
    (void)severity;
    (void)length;
    (void)userParam;
    std::cout << msg << '\n';
    std::flush(std::cout);
}
int main(int argc, char *argv[]) {
    glfwInit();
    // Define required version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Specify that the project uses modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // Debug
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    // Creation of the window
    GLFWwindow *win =
        glfwCreateWindow(screenSize.x, screenSize.y, "FEngine", nullptr, nullptr);
    FAIL_ON(win == nullptr, "An error occured while creating the window");
    glfwMakeContextCurrent(win);

    FAIL_ON(glewInit() != GLEW_OK, "Glew could not be initialized");
    glViewport(0, 0, screenSize.x, screenSize.y);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugCallback, 0);

    /* sum(); */

    Render render =
        Render("assets/shaders/default.vert", "assets/shaders/default.frag");

    Mesh m = Mesh::createFrom("assets/sphere.obj");
    const float radius = argc == 1 ? 3.0f : atoi(argv[1]);
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            Object &o = m.createObject();
            *o.getColor() = vec4(1.0f);
            Transform t = o.getTransform();
            t.position = vec3(i * offset.x, j * offset.y, 0.0f);
            t.position -=
                vec3(offset.x * size.x / 2, offset.y * size.y / 2, 0.0f);
            t.scale = vec3(radius / 3, radius / 3, 0.0f);
            o.setTransform(t);
        }
    }
    const int objNb = m.getObjects().size();

    Mesh bbox_m = Mesh::createFrom("assets/square.obj");
    Object bbox = bbox_m.createObject();
    auto bbox_t = bbox.getTransform();
    bbox_t.scale = bounds + vec3(radius);
    bbox_t.position.z = -1.0f;
    bbox.setTransform(bbox_t);
    *bbox.getColor() = vec4(1.0f, 0.1f, 0.1f, 1.0f);

    Compute grav("assets/shaders/gravity.comp");
    vec3 ubound = bounds;
    vec3 lbound = -bounds;
    glUniform3fv(grav.getUniformLoc("ubound"), 1, (GLfloat *)&ubound);
    glUniform3fv(grav.getUniformLoc("lbound"), 1, (GLfloat *)&lbound);
    glUniform3fv(grav.getUniformLoc("interaction"), 1,
                 (float *)&Camera::mainCamera().interactionPoint);
    glUniform1f(grav.getUniformLoc("radius"), radius);
    grav.setupData(Object::getTransforms(m), objNb, sizeof(mat4), 0,
                   GL_DYNAMIC_DRAW);
    grav.setupData(Object::getVelocities(m), objNb, sizeof(vec4), 1,
                   GL_DYNAMIC_DRAW);
    grav.setupData(Object::getColors(m), objNb, sizeof(vec4), 2,
                   GL_DYNAMIC_DRAW);

    /* exit(1); */

    glfwSetTime(0);
    double last = glfwGetTime();
    double lastSec = glfwGetTime();
    int fps = 0;

    glEnable(GL_DEPTH_TEST);
    Camera::mainCamera().transform.position.z = -13.5;
    // Main loop
    while (!glfwWindowShouldClose(win)) {
        // Take care of events
        glfwPollEvents();
        // Background color
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double cur = glfwGetTime();
        double dt = cur - last;
        last = cur;
        glUniform1f(grav.getUniformLoc("deltaTime"), static_cast<float>(dt));
        if (cur - lastSec >= 1.0) {
            lastSec += 1.0;
            std::cout << "FPS: " << fps << '\n';
            fps = 0;
        }
        fps++;

        Camera::mainCamera().inputs();

        m.render(render, Camera::mainCamera());
        bbox_m.render(render, Camera::mainCamera());

        glUniform3fv(grav.getUniformLoc("interaction"), 1,
                     (float *)&Camera::mainCamera().interactionPoint);
        glUniform1i(grav.getUniformLoc("inputState"),
                    Camera::mainCamera().clickState);
        grav.updateData(Object::getTransforms(m), 0);
        grav.updateData(Object::getVelocities(m), 1);
        grav.updateData(Object::getColors(m), 2);
        grav.dispatch(objNb);

        const mat4 *newpos = (const mat4 *)grav.retrieveData(0);
        memcpy(Object::getTransforms(m), newpos, objNb * sizeof(mat4));
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        const vec4 *newvel = (const vec4 *)grav.retrieveData(1);
        memcpy(Object::getVelocities(m), newvel, objNb * sizeof(vec4));
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        const vec4 *newcol = (const vec4 *)grav.retrieveData(2);
        memcpy(Object::getColors(m), newcol, objNb * sizeof(vec4));
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        glfwSwapBuffers(win);
    }

    clean_exit(0);
}

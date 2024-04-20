#include <algorithm>
#include <iostream>
#include <GL/glew.h>
#include "mesh/mesh.hh"
#include "object/camera.hh"
#include "object/object.hh"
#include "shader.hh"
#include "tools.hh"
#include <GLFW/glfw3.h>
#include <string.h>

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

int main() {
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
        glfwCreateWindow(WIDTH, HEIGHT, "FEngine", nullptr, nullptr);
    FAIL_ON(win == nullptr, "An error occured while creating the window");
    glfwMakeContextCurrent(win);

    FAIL_ON(glewInit() != GLEW_OK, "Glew could not be initialized");
    glViewport(0, 0, WIDTH, HEIGHT);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugCallback, 0);

    Shader shader =
        Shader("assets/shaders/default.vert", "assets/shaders/default.frag");

    Mesh m = Mesh::createFrom("assets/sphere.obj");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                Object &o = m.createObject();
                Transform t = o.getTransform();
                t.position = vec3(i * 2.0f, j * 2.0f, -2.0f * k);
                o.setTransform(t);
            }
        }
    }

    glfwSetTime(0);
    double last = glfwGetTime();
    int fps = 0;

    glEnable(GL_DEPTH_TEST);
    // Main loop
    while (!glfwWindowShouldClose(win)) {
        // Take care of events
        glfwPollEvents();
        // Background color
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetTime() - last >= 1.0) {
            last += 1.0;
            std::cout << "FPS: " << fps << '\n';
            fps = 0;
        }
        fps++;

        shader.activate();
        Camera::mainCamera().inputs(win);
        m.render(shader, Camera::mainCamera());

        /* for (Object &o : m.getObjects()) { */
        /*     Transform t = o.getTransform(); */
        /*     t.position.y -= 0.1; */
        /*     /1* t.position.y = std::max(-10.0f, t.position.y); *1/ */
        /*     o.setTransform(t); */
        /* } */

        glfwSwapBuffers(win);
    }

    clean_exit(0);
}

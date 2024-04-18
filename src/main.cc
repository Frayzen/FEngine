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
    /* glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); */

    // Creation of the window
    GLFWwindow *win =
        glfwCreateWindow(WIDTH, HEIGHT, "FEngine", nullptr, nullptr);
    FAIL_ON(win == nullptr, "An error occured while creating the window");
    glfwMakeContextCurrent(win);

    FAIL_ON(glewInit() != GLEW_OK, "Glew could not be initialized");
    glViewport(0, 0, WIDTH, HEIGHT);

    /* glEnable(GL_DEBUG_OUTPUT); */
    /* glDebugMessageCallback(DebugCallback, 0); */

    Shader shader =
        Shader("assets/shaders/default.vert", "assets/shaders/default.frag");

    Mesh m = Mesh::createFrom("assets/teddy.obj");
    Object o = Object(m);
    o.transform.position = vec3(0.0f, 0.0f, -8.0f);
    o.transform.scale = vec3(0.3f);

    glEnable(GL_DEPTH_TEST);
    // Main loop
    while (!glfwWindowShouldClose(win)) {
        // Take care of events
        glfwPollEvents();
        // Background color
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.activate();
        /* glDrawArrays(GL_TRIANGLES, 0, 3); */
        Camera::mainCamera().inputs(win);
        o.render(shader, Camera::mainCamera());

        glfwSwapBuffers(win);
    }

    clean_exit(0);
}

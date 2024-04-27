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
    GLFWwindow *win = glfwCreateWindow(screenSize.x, screenSize.y, "FEngine",
                                       nullptr, nullptr);
    FAIL_ON(win == nullptr, "An error occured while creating the window");
    glfwMakeContextCurrent(win);

    FAIL_ON(glewInit() != GLEW_OK, "Glew could not be initialized");
    FAIL_ON(!GLEW_ARB_framebuffer_object,
            "Error: GL_ARB_framebuffer_object extension is not supported");
    glViewport(0, 0, screenSize.x, screenSize.y);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugCallback, 0);

    Render render =
        Render("assets/shaders/default.vert", "assets/shaders/default.frag");

    Mesh mesh = Mesh::createFrom("assets/airplane/airplane.obj");
    auto obj = mesh.createObject();
    auto t = obj.getTransform();
    t.scale = vec3(0.01f);
    obj.setTransform(t);

    /* Mesh iron = Mesh::createFrom("assets/ironman/IronMan.obj"); */
    /* Object irono = iron.createObject(); */
    /* t = obj.getTransform(); */
    /* t.position.z -= 50.0f; */
    /* t.rotation = */
    /*     glm::rotate(t.rotation, glm::radians(-90.0f), vec3(-1.0f, 0, 0)); */
    /* t.scale = vec3(0.1f); */
    /* irono.setTransform(t); */

    Mesh sph = Mesh::createFrom("assets/sphere.obj");
    Object spho = sph.createObject();
    Transform st = spho.getTransform();
    st.position = lightPos;
    spho.setTransform(st);

    glfwSetTime(0);
    double lastSec = glfwGetTime();
    int fps = 0;

    glEnable(GL_DEPTH_TEST);

    // Main loop
    while (!glfwWindowShouldClose(win)) {
        // Take care of events
        glfwPollEvents();
        // Background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double cur = glfwGetTime();
        if (cur - lastSec >= 1.0) {
            lastSec += 1.0;
            std::cout << "FPS: " << fps << '\n';
            fps = 0;
        }
        fps++;

        Camera::mainCamera().inputs();
        mesh.render(render, Camera::mainCamera());
        /* iron.render(render, Camera::mainCamera()); */
        sph.render(render, Camera::mainCamera());

        glfwSwapBuffers(win);
    }

    clean_exit(0);
}

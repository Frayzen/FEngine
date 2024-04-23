#include <algorithm>
#include <glm/fwd.hpp>
#include <iostream>
#include <GL/glew.h>
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

void sum() {
    Compute cmp = Compute("assets/shaders/summer.comp");
    const int nb = 10;
    float a[nb];
    float b[nb];
    for (int i = 0; i < nb; i++) {
        a[i] = 1.0f * i * i;
        b[i] = 1.0f * i;
    }
    cmp.setupData(a, nb, sizeof(float), 1, GL_DYNAMIC_COPY);
    cmp.setupData(b, nb, sizeof(float), 2, GL_DYNAMIC_COPY);
    cmp.dispatch(nb);
    auto res = reinterpret_cast<const float *>(cmp.retrieveData(2));
    for (int i = 0; i < nb; i++)
        std::cout << res[i] << ' ';
    std::cout << std::endl;
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

    /* sum(); */

    Render render =
        Render("assets/shaders/default.vert", "assets/shaders/default.frag");

    Mesh m = Mesh::createFrom("assets/sphere.obj");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                Object &o = m.createObject();
                Transform t = o.getTransform();
                t.position = vec3(i * 3.0f, j * 3.0f, -3.0f * k);
                o.setTransform(t);
            }
        }
    }

    const int objNb = m.getObjects().size();

    Compute grav("assets/shaders/gravity.comp");
    vec3 ubound = {20.0f, 20.0f, 20.0f};
    vec3 lbound = {-20.0f, -20.0f, -20.0f};
    glUniform3fv(grav.getUniformLoc("ubound"), 1, (GLfloat *)&ubound);
    glUniform3fv(grav.getUniformLoc("lbound"), 1, (GLfloat *)&lbound);
    grav.setupData(Object::getTransforms(m), objNb, sizeof(mat4), 0,
                   GL_DYNAMIC_DRAW);
    grav.setupData(Object::getVelocities(m), objNb, sizeof(vec4), 1,
                   GL_DYNAMIC_DRAW);

    /* exit(1); */

    glfwSetTime(0);
    double last = glfwGetTime();
    int fps = 0;

    glEnable(GL_DEPTH_TEST);
    Camera::mainCamera().transform.position.z = -30;
    // Main loop
    while (!glfwWindowShouldClose(win)) {
        // Take care of events
        glfwPollEvents();
        // Background color
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double dt = glfwGetTime() - last;
        glUniform1f(grav.getUniformLoc("deltaTime"), static_cast<float>(dt));
        if (dt >= 1.0) {
            last += 1.0;
            std::cout << "FPS: " << fps << '\n';
            fps = 0;
        }
        fps++;

        render.activate();
        Camera::mainCamera().inputs(win);
        m.render(render, Camera::mainCamera());

        grav.updateData(Object::getTransforms(m), 0);
        grav.updateData(Object::getVelocities(m), 1);
        grav.dispatch(objNb);
        const mat4 *newpos = (const mat4 *)grav.retrieveData(0);
        memcpy(Object::getTransforms(m), newpos, objNb * sizeof(mat4));

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        const vec4 *newvel = (const vec4 *)grav.retrieveData(1);
        memcpy(Object::getVelocities(m), newvel, objNb * sizeof(vec4));
        
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        glfwSwapBuffers(win);
    }

    clean_exit(0);
}

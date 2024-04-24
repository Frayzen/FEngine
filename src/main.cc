#include <algorithm>
#include <cstdlib>
#include <glm/fwd.hpp>
#include <iostream>
#include <GL/glew.h>
#include "constants.hh"
#include "gui/imgui.h"
#include "gui/imgui_impl_glfw.h"
#include "gui/imgui_impl_opengl3.h"
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

void updateGUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and
                                   // append into it.

    ImGui::Text("This is some useful text."); // Display some text (you can use
                                              // a format strings too)
    bool win, show_another_window;
    ImGui::Checkbox("Demo Window", &win);
    // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f,
                       1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
    vec3 clear_color;
    ImGui::ColorEdit3(
        "clear color",
        (float *)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button")) // Buttons return true when clicked (most
                                 // widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGuiIO &io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

void setupGUI(GLFWwindow *win) {
    const char *glsl_version = "#version 430";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(win, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);
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
    GLFWwindow *win = glfwCreateWindow(screenSize.x, screenSize.y, "FEngine",
                                       nullptr, nullptr);
    FAIL_ON(win == nullptr, "An error occured while creating the window");
    glfwMakeContextCurrent(win);

    FAIL_ON(glewInit() != GLEW_OK, "Glew could not be initialized");
    glViewport(0, 0, screenSize.x, screenSize.y);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugCallback, 0);

    setupGUI(win);

    /* sum(); */

    Render render =
        Render("assets/shaders/default.vert", "assets/shaders/default.frag");

    Mesh m = Mesh::createFrom("assets/sphere.obj");
    const float radius = argc == 1 ? 1.0f : std::stof(argv[1]);
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            Object &o = m.createObject();
            *o.getColor() = vec4(1.0f);
            Transform t = o.getTransform();
            t.position = vec3(i * offset.x, j * offset.y, 0.0f);
            t.position -=
                vec3(offset.x * size.x / 2, offset.y * size.y / 2, 0.0f);
            t.scale = vec3(radius / 10, radius / 10, 0.0f);
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

    Compute dens("assets/shaders/density.comp");
    glUniform1f(dens.getUniformLoc("radius"), radius);
    glUniform1f(dens.getUniformLoc("mass"), mass);
    dens.setupData(Object::getTransforms(m), objNb, sizeof(mat4), 0,
                   GL_DYNAMIC_DRAW);
    dens.setupData(Object::getVelocities(m), objNb, sizeof(vec4), 1,
                   GL_DYNAMIC_DRAW);

    Compute grav("assets/shaders/gravity.comp");
    vec3 ubound = bounds;
    vec3 lbound = -bounds;
    grav.setupData(0, dens.getBuffer(0));
    grav.setupData(1, dens.getBuffer(1));
    grav.setupData(Object::getColors(m), objNb, sizeof(vec4), 2,
                   GL_DYNAMIC_DRAW);
    glUniform3fv(grav.getUniformLoc("ubound"), 1, (GLfloat *)&ubound);
    glUniform3fv(grav.getUniformLoc("lbound"), 1, (GLfloat *)&lbound);
    glUniform3fv(grav.getUniformLoc("interaction"), 1,
                 (float *)&Camera::mainCamera().interactionPoint);
    glUniform1f(grav.getUniformLoc("radius"), radius);
    glUniform1f(grav.getUniformLoc("mass"), mass);

    /* exit(1); */

    glfwSetTime(0);
    double last = glfwGetTime();
    double lastSec = glfwGetTime();
    int fps = 0;

    glEnable(GL_DEPTH_TEST);
    Camera::mainCamera().transform.position.z = -12;
    // Main loop
    while (!glfwWindowShouldClose(win)) {
        // Take care of events
        glfwPollEvents();

        updateGUI();

        // Background color
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double cur = glfwGetTime();
        double dt = cur - last;
        last = cur;
        glUniform1f(grav.getUniformLoc("deltaTime"), static_cast<float>(dt));
        glUniform1f(dens.getUniformLoc("deltaTime"), static_cast<float>(dt));
        if (cur - lastSec >= 1.0) {
            lastSec += 1.0;
            std::cout << "FPS: " << fps << '\n';
            fps = 0;
        }
        fps++;

        Camera::mainCamera().inputs();

        m.render(render, Camera::mainCamera());
        bbox_m.render(render, Camera::mainCamera());

        dens.dispatch(objNb);

        glUniform3fv(grav.getUniformLoc("interaction"), 1,
                     (float *)&Camera::mainCamera().interactionPoint);
        glUniform1i(grav.getUniformLoc("inputState"),
                    Camera::mainCamera().clickState);
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

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(win);
    }

    clean_exit(0);
}

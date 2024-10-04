#include <imgui_impl_opengl3.h>
#include "simulation.hh"
#include "constants.hh"
#include "gui/gui.hh"
#include "tools.hh"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstring>

#define OBJNB(Mesh) (Mesh.getObjects().size())

void Simulation::restartSimulation() {
    isRunning = false;
    for (auto &m : meshes_)
        m.get().clearObjects();
    init();
}

void Simulation::setup() {
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
    glEnable(GL_DEPTH_TEST);

    std::cout << "OPENGL VERSION: " << glGetString(GL_VERSION) << std::endl;
}

Simulation::Simulation()
    : renderer_(
          Render("assets/shaders/default.vert", "assets/shaders/default.frag")),
      win_(glfwGetCurrentContext()) {
    assert(glfwGetCurrentContext() != nullptr);
    glfwSetTime(0);
    lastTime_ = glfwGetTime();
}

void Simulation::registerMesh(Mesh &m) { meshes_.push_back(m); }

// Main loop
void Simulation::run() {
    init();
    while (!glfwWindowShouldClose(win_)) {

        // BACKGROUND
        glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TIME UPDATE
        double curTime = glfwGetTime();
        double deltaTime = curTime - lastTime_;
        lastTime_ = curTime;

        // EVENTS
        glfwPollEvents();

        cam.inputs();
        update(deltaTime);

        // RENDER
        for (unsigned long i = 0; i < meshes_.size(); i++)
            meshes_[i].get().render(renderer_, cam);

        if (gui_ != nullptr) {

            gui_->update();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(win_);
    }
    glfwTerminate();
    if (glfwGetCurrentContext() != nullptr)
        glfwDestroyWindow(glfwGetCurrentContext());
}

void Simulation::attachGUI(GUI *gui) {
    gui_ = gui;
    if (gui_ == nullptr)
        return;
    gui->setup();
}

std::vector<std::reference_wrapper<Object>> Simulation::getObjects() {
    std::vector<std::reference_wrapper<Object>> objects;
    for (auto &m : meshes_)
        objects.insert(objects.end(), m.get().getObjects().begin(),
                       m.get().getObjects().end());
    return objects;
}

Simulation::~Simulation()
{
    delete(gui_);	
}

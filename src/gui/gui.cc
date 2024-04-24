#include "gui/imgui.h"
#include "simulation/simulation.hh"
#include "gui.hh"
#include "gui/imgui_impl_glfw.h"
#include "gui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

GUI::GUI(Simulation &sim) : win_(glfwGetCurrentContext()), sim_(sim) {}

static bool created = false;
void GUI::update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("FEngine_t");
    if (!created) {
        created = true;
        ImGui::SetWindowFontScale(1.8f);
        ImGui::SetWindowPos(ImVec2(50, 50));
        ImGui::SetWindowSize(ImVec2(300, 500));
    }
    ImGui::SliderFloat2("Bounds", (float *)&sim_.bounds, 5.0f, 100.0f);
    ImGui::SliderFloat("Mass", (float *)&sim_.mass, 0.0f, 5.0f);
    ImGui::SliderFloat("Pressure Multiplier", (float *)&sim_.pressureMultiplier,
                       0.0f, 500.0f);
    if (ImGui::Button("Restart"))
        sim_.restartSimulation();

    if (ImGui::Button(sim_.isRunning ? "Pause" : "Play"))
        sim_.isRunning = !sim_.isRunning;


    ImGuiIO &io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

static bool isSetup = false;
void GUI::setup() {
    if (isSetup)
        return;
    isSetup = true;
    const char *glsl_version = "#version 430";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(win_, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);
}

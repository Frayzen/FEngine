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
    ImGui::SliderFloat("Mass", &sim_.mass, 0.0f, 5.0f);
    ImGui::SliderFloat("Targeted Density", &sim_.targetedDensity, 1.0f, 5.0f);
    ImGui::DragFloat3("Gravity", &sim_.gravity[0], -0.1f, 0.1f);
    ImGui::SliderFloat("Viscosity", (float *)&sim_.viscosity, 0.2f, 10.0f);
    ImGui::SliderFloat("Pressure Multiplier", (float *)&sim_.pressureMultiplier,
                       0.0f, 500.0f);
    if (ImGui::InputFloat("Radius", &sim_.radius)) {
        for (Object &o : sim_.getParticles()) {
            auto p = o.getTransform().position;
            auto t = Transform::identity();
            t.position = p;
            t.scale = vec3(sim_.radius * sim_.appearanceRadiusCoeff,
                           sim_.radius * sim_.appearanceRadiusCoeff, 0.0f);
            o.setTransform(t);
        }
    }
    ImGui::Spacing();
    if (ImGui::InputInt2("Number", (int *)&sim_.size))
        sim_.restartSimulation();
    if (ImGui::InputFloat2("Offset", (float *)&sim_.offset))
        sim_.restartSimulation();
    static bool pPressStep = false;
    bool pressStep =
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS;
    if (ImGui::Button("Step") || (pressStep && !pPressStep)) {
        sim_.isRunning = false;
        sim_.step();
    }
    pPressStep = pressStep;

    static bool pPressRestart = false;
    bool pressRestart =
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_BACKSPACE) == GLFW_PRESS;
    if (ImGui::Button("Restart") || (pressRestart && !pPressRestart))
        sim_.restartSimulation();
    pPressRestart = pressRestart;

    static bool pPressPause = false;
    bool pressPause =
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    if (ImGui::Button("Pause") || (pressPause && !pPressPause))
        sim_.isRunning = !sim_.isRunning;
    pPressPause = pressPause;

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

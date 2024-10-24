#include "fem_gui.hh"
#include "simulation/fem/fem_sim.hh"
#include <imgui.h>

static bool created = false;
void FemGUI::update() {
    FemSimulation &sim = (FemSimulation &)sim_;
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
    if (ImGui::Button("Clear"))
        sim.restartSimulation();
    if (ImGui::Button("Compute"))
        sim.computeMesh();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

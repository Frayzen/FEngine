#include "simulation/fem/fem_2d_mesh.hh"
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
    FEMPoint *cur = sim.getCurrentPoint();
    if (cur) {
        if (cur->flags != FIXED)
            ImGui::Text("Z: point fixed");
        else
            ImGui::Text("Z: point free");
        if (cur->flags != ROLLING_X)
            ImGui::Text("X: point roller X");
        else
            ImGui::Text("X: point free");
        if (cur->flags != ROLLING_Y)
            ImGui::Text("C: point roller Y");
        else
            ImGui::Text("C: point free");

        switch (cur->flags) {
        case NONE:
            if (ImGui::SliderFloat2("Force applied",
                                    (float *)&cur->forceApplied, -500, 500))
                sim.computeMesh();
            break;
        case ROLLING_X:
            if (ImGui::SliderFloat("Force X applied", &cur->forceApplied.x,
                                   -500, 500))
                sim.computeMesh();
            break;
        case ROLLING_Y:
            if (ImGui::SliderFloat("Force Y applied", &cur->forceApplied.y,
                                   -500, 500))
                sim.computeMesh();
            break;
        default:
            break;
        }
    }
    if (ImGui::Button("Reset All Forces"))
        sim.fem_mesh_.resetForces();

    ImGuiIO &io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

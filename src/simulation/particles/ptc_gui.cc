#include "simulation/particles/ptc_sim.hh"
#include "ptc_gui.hh"

static bool created = false;
void PtcGUI::update() {
    PtcSimulation &sim = (PtcSimulation &)sim_;
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
    ImGui::SliderFloat2("Bounds", (float *)&sim.bounds, 5.0f, 100.0f);
    ImGui::SliderFloat("Mass", &sim.mass, 0.0f, 5.0f);
    ImGui::SliderFloat("Targeted Density", &sim.targetedDensity, 0.0f, 5.0f);
    ImGui::SliderFloat("Gravity", &sim.gravity, -10.0f, 10.0f);
    ImGui::SliderFloat("Pressure Multiplier", (float *)&sim.pressureMultiplier,
                       0.0f, 500.0f);
    if (ImGui::InputFloat("Radius", &sim.radius)) {
        for (Object &o : sim.getParticles()) {
            auto p = o.getTransform().position;
            auto t = Transform::identity();
            t.position = p;
            t.scale = vec3(sim.radius * sim.appearanceRadiusCoeff,
                           sim.radius * sim.appearanceRadiusCoeff, 0.0f);
            o.setTransform(t);
        }
    }
    ImGui::Spacing();
    if (ImGui::SliderInt2("Number", (int *)&sim.size, 1, 300))
        sim.restartSimulation();
    if (ImGui::SliderFloat2("Offset", (float *)&sim.offset, 0.0f, 3.0f))
        sim.restartSimulation();
    if (ImGui::Button("Restart"))
        sim.restartSimulation();

    if (ImGui::Button(sim.isRunning ? "Pause" : "Play"))
        sim.isRunning = !sim.isRunning;

    ImGuiIO &io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

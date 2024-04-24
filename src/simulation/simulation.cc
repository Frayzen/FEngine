#include "simulation.hh"
#include "gui/imgui.h"
#include "gui/imgui_impl_opengl3.h"
#include "mesh/mesh.hh"
#include "shader/render.hh"
#include <cstring>
#include <iostream>

#define OBJNB (particleMesh_.getObjects().size())
#define UBOUNDS (bounds)
#define LBOUNDS (-bounds)

void Simulation::restartSimulation() {
    isRunning = false;
    particleMesh_.getObjects().clear();
    createObjects();
    updateBuffers();
}

Simulation::Simulation()
    : gui_(GUI(*this)), particleMesh_(Mesh::createFrom("assets/sphere.obj")),
      boundingMesh_(Mesh::createFrom("assets/square.obj")),
      densityCmpt_(Compute("assets/shaders/density.comp")),
      velocityCpt_(Compute("assets/shaders/velocity.comp")),
      renderer_(
          Render("assets/shaders/default.vert", "assets/shaders/default.frag")),
      win_(glfwGetCurrentContext()) {
    gui_.setup();

    createObjects();
    densityCmpt_.setupData(Object::getTransforms(particleMesh_), OBJNB,
                           sizeof(mat4), 0, GL_DYNAMIC_DRAW);
    densityCmpt_.setupData(Object::getVelocities(particleMesh_), OBJNB,
                           sizeof(vec4), 1, GL_DYNAMIC_DRAW);
    velocityCpt_.setupData(Object::getColors(particleMesh_), OBJNB,
                           sizeof(vec4), 2, GL_DYNAMIC_DRAW);
    velocityCpt_.setupData(0, densityCmpt_.getBuffer(0));
    velocityCpt_.setupData(1, densityCmpt_.getBuffer(1));
    boundingMesh_.createObject();
    updateBbox();

    glfwSetTime(0);
    last_ = glfwGetTime();
}

void Simulation::updateBuffers()
{
    velocityCpt_.updateData(Object::getTransforms(particleMesh_), 0);
    velocityCpt_.updateData(Object::getVelocities(particleMesh_), 1);
    velocityCpt_.updateData(Object::getColors(particleMesh_), 2);
}

void Simulation::createObjects() {
    particleMesh_.clearObjects();
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            Object &o = particleMesh_.createObject();
            *o.getColor() = vec4(1.0f);
            Transform t = Transform::identity();
            t.position = vec3(i * offset.x, j * offset.y, 0.0f);
            t.position -=
                vec3(offset.x * size.x / 2, offset.y * size.y / 2, 0.0f);
            t.scale = vec3(radius / 3, radius / 3, 0.0f);
            o.setTransform(t);
        }
    }
}

void Simulation::updateBbox() {
    auto bbox = boundingMesh_.getObjects()[0];
    auto bbox_t = bbox.getTransform();
    bbox_t.scale = bounds + vec3(radius);
    bbox_t.position.z = -1.0f;
    bbox.setTransform(bbox_t);
    *bbox.getColor() = vec4(1.0f, 0.1f, 0.1f, 1.0f);
}

void Simulation::compute() {

    // COMPUTE DENSITY
    glUniform1f(densityCmpt_.getUniformLoc("radius"), radius);
    glUniform1f(densityCmpt_.getUniformLoc("mass"), mass);
    densityCmpt_.dispatch(OBJNB);

    // COMPUTE VELOCITY
    velocityCpt_.updateData(Object::getTransforms(particleMesh_), 0);
    velocityCpt_.updateData(Object::getVelocities(particleMesh_), 1);
    velocityCpt_.updateData(Object::getColors(particleMesh_), 2);
    glUniform3fv(velocityCpt_.getUniformLoc("interaction"), 1,
                 (float *)&cam.interactionPoint);
    glUniform1i(velocityCpt_.getUniformLoc("inputState"), cam.clickState);
    glUniform1f(velocityCpt_.getUniformLoc("radius"), radius);
    glUniform1f(velocityCpt_.getUniformLoc("mass"), mass);
    glUniform3f(velocityCpt_.getUniformLoc("ubound"), UBOUNDS.x, UBOUNDS.y,
                UBOUNDS.z);
    glUniform3f(velocityCpt_.getUniformLoc("lbound"), LBOUNDS.x, LBOUNDS.y,
                LBOUNDS.z);
    velocityCpt_.dispatch(OBJNB);
    // RETRIEVE DATA
    memcpy(Object::getTransforms(particleMesh_), velocityCpt_.retrieveData(0),
           OBJNB * sizeof(mat4));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    memcpy(Object::getVelocities(particleMesh_), velocityCpt_.retrieveData(1),
           OBJNB * sizeof(vec4));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    memcpy(Object::getColors(particleMesh_), velocityCpt_.retrieveData(2),
           OBJNB * sizeof(vec4));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

// Main loop
void Simulation::mainLoop() {
    while (!glfwWindowShouldClose(win_)) {

        // BACKGROUND
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // INPUTS
        double cur = glfwGetTime();
        glUniform1f(velocityCpt_.getUniformLoc("deltaTime"), cur - last_);
        last_ = cur;
        glfwPollEvents();
        gui_.update();
        cam.inputs(vec2(bounds.x, bounds.y));

        if (isRunning)
            compute();

        // RENDER
        particleMesh_.render(renderer_, cam);
        updateBbox();
        boundingMesh_.render(renderer_, cam);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(win_);
    }
}

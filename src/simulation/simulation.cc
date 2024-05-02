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
    createObjects();
    setupBuffers();
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
    setupBuffers();
    boundingMesh_.createObject();
    updateBbox();

    glfwSetTime(0);
    last_ = glfwGetTime();
}

void Simulation::setupBuffers() {
    // SETUP DENS
    densityCmpt_.setupData(Object::getTransforms(particleMesh_),
                           OBJNB * sizeof(mat4), 0, GL_DYNAMIC_DRAW);
    densityCmpt_.setupData(Object::getVelocities(particleMesh_),
                           OBJNB * sizeof(vec4), 1, GL_DYNAMIC_DRAW);
    // SETUP VEL
    velocityCpt_.setupData(Object::getTransforms(particleMesh_),
                           OBJNB * sizeof(mat4), 0, GL_DYNAMIC_DRAW);
    velocityCpt_.setupData(Object::getVelocities(particleMesh_),
                           OBJNB * sizeof(vec4), 1, GL_DYNAMIC_DRAW);
    velocityCpt_.setupData(Object::getColors(particleMesh_),
                           OBJNB * sizeof(vec4), 2, GL_DYNAMIC_DRAW);
}

void Simulation::updateBuffers() {
    // DENSITY UPDATE
    velocityCpt_.updateData(Object::getTransforms(particleMesh_), 0);
    velocityCpt_.updateData(Object::getVelocities(particleMesh_), 1);
    velocityCpt_.updateData(Object::getColors(particleMesh_), 2);
    // DENSITY UPDATE
    densityCmpt_.updateData(Object::getTransforms(particleMesh_), 0);
    densityCmpt_.updateData(Object::getVelocities(particleMesh_), 1);
}

void spawnParticle(Mesh &m, float x, float y, float dimension) {

    Object &o = m.createObject();
    *o.getColor() = vec4(1.0f);
    Transform t = Transform::identity();
    t.position = vec3(x, y, 0.0f);
    if (dimension == -1) {
        // BOUNDING BALLS
        t.scale = vec3(0);
        *o.getColor() = vec4(0);
    } else
        t.scale = vec3(dimension);
    o.setTransform(t);
}

void Simulation::createObjects() {
    particleMesh_.clearObjects();
    const float dist = radius;
    const int nbx = 2 + bounds.x / dist;
    const int nby = 2 + bounds.y / dist;
    const float bx = bounds.x + dist / 2;
    const float by = bounds.y + dist / 2;
    for (int i = 0; i < 2 * nbx; i++) {
        spawnParticle(particleMesh_, -bx + i * dist, -by, -1);
        spawnParticle(particleMesh_, -bx + i * dist, by, -1);
    }
    for (int i = 0; i < 2 * nby; i++) {
        spawnParticle(particleMesh_, -bx, -by + i * dist, -1);
        spawnParticle(particleMesh_, bx, -by + i * dist, -1);
    }
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            spawnParticle(particleMesh_, offset.x * (i - size.x / 2),
                          offset.y * (j - size.y / 2),
                          radius * appearanceRadiusCoeff);
        }
    }
}
void Simulation::updateBbox() {
    auto bbox = boundingMesh_.getObjects()[0];
    auto bbox_t = bbox.getTransform();
    bbox_t.scale = bounds + vec3(radius);
    bbox_t.position.z = -1.0f;
    bbox.setTransform(bbox_t);
    *bbox.getColor() = vec4(0.1f, 0.1f, 0.1f, 1.0f);
}

void Simulation::step() {

    // COMPUTE DENSITY
    densityCmpt_.updateData(Object::getTransforms(particleMesh_), 0);

    densityCmpt_.updateData(Object::getVelocities(particleMesh_), 1);
    glUniform1f(densityCmpt_.getUniformLoc("radius"), radius);
    glUniform1f(densityCmpt_.getUniformLoc("mass"), mass);
    densityCmpt_.dispatch(OBJNB);

    vec4 *vels = (vec4 *)velocityCpt_.retrieveData(1);
    memcpy(Object::getVelocities(particleMesh_), vels, OBJNB * sizeof(vec4));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    // COMPUTE VELOCITY
    velocityCpt_.updateData(Object::getTransforms(particleMesh_), 0);
    velocityCpt_.updateData(Object::getVelocities(particleMesh_), 1);
    velocityCpt_.updateData(Object::getColors(particleMesh_), 2);
    glUniform3fv(velocityCpt_.getUniformLoc("interaction"), 1,
                 (float *)&cam.interactionPoint);
    glUniform1i(velocityCpt_.getUniformLoc("inputState"), cam.clickState);
    glUniform1f(velocityCpt_.getUniformLoc("radius"), radius);
    glUniform1f(velocityCpt_.getUniformLoc("mass"), mass);
    glUniform1f(velocityCpt_.getUniformLoc("pressureMultiplier"), pressureMultiplier);
    glUniform1f(velocityCpt_.getUniformLoc("targetedDensity"), targetedDensity);
    glUniform3fv(velocityCpt_.getUniformLoc("gravity"), 1, &gravity[0]);
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
        /* glUniform1f(velocityCpt_.getUniformLoc("deltaTime"), cur - last_); */
        /* glUniform1f(densityCmpt_.getUniformLoc("deltaTime"), cur - last_); */
        last_ = cur;
        glfwPollEvents();
        gui_.update();
        cam.inputs(vec2(bounds.x, bounds.y));

        if (isRunning)
            step();

        // RENDER
        particleMesh_.render(renderer_, cam);
        updateBbox();
        boundingMesh_.render(renderer_, cam);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(win_);
    }
}

std::vector<Object> &Simulation::getParticles() {
    return particleMesh_.getObjects();
}

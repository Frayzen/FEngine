#include "ptc_simulation.hh"
#include "mesh/mesh.hh"
#include <cstring>
#include <iostream>

#define OBJNB (particleMesh_.getObjects().size())
#define UBOUNDS (bounds)
#define LBOUNDS (-bounds)

void PtcSimulation::update(double deltaTime) {
    glUniform1f(velocityCpt_.getUniformLoc("deltaTime"), deltaTime);
    glUniform1f(densityCmpt_.getUniformLoc("deltaTime"), deltaTime);
    cam.inputs(vec2(bounds.x, bounds.y));
    updateBbox();
}
void PtcSimulation::init() {
    createObjects();
    setupBuffers();
}

PtcSimulation::PtcSimulation()
    : Simulation(nullptr), particleMesh_(Mesh::createFrom("assets/sphere.obj")),
      boundingMesh_(Mesh::createFrom("assets/square.obj")),
      densityCmpt_(Compute("assets/shaders/density.comp")),
      velocityCpt_(Compute("assets/shaders/velocity.comp")) {
    registerMesh(particleMesh_);
    registerMesh(boundingMesh_);
}

void PtcSimulation::setupBuffers() {
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

void PtcSimulation::createObjects() {
    boundingMesh_.createObject();
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            Object &o = particleMesh_.createObject();
            *o.getColor() = vec4(1.0f);
            Transform t = Transform::identity();
            t.position = vec3(i * offset.x, j * offset.y, 0.0f);
            t.position -=
                vec3(offset.x * size.x / 2, offset.y * size.y / 2, 0.0f);
            t.scale = vec3(radius * appearanceRadiusCoeff,
                           radius * appearanceRadiusCoeff, 0.0f);
            o.setTransform(t);
        }
    }
}

void PtcSimulation::updateBuffers() {
    // DENSITY UPDATE
    velocityCpt_.updateData(Object::getTransforms(particleMesh_), 0);
    velocityCpt_.updateData(Object::getVelocities(particleMesh_), 1);
    velocityCpt_.updateData(Object::getColors(particleMesh_), 2);
    // DENSITY UPDATE
    densityCmpt_.updateData(Object::getTransforms(particleMesh_), 0);
    densityCmpt_.updateData(Object::getVelocities(particleMesh_), 1);
}

void PtcSimulation::updateBbox() {
    auto bbox = boundingMesh_.getObjects()[0];
    auto bbox_t = bbox.getTransform();
    bbox_t.scale = bounds + vec3(radius);
    bbox_t.position.z = -1.0f;
    bbox.setTransform(bbox_t);
    *bbox.getColor() = vec4(0.1f, 0.1f, 0.1f, 1.0f);
}

void PtcSimulation::compute() {

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
    glUniform1f(velocityCpt_.getUniformLoc("targetedDensity"), targetedDensity);
    glUniform1f(velocityCpt_.getUniformLoc("gravity"), gravity);
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

std::vector<Object> PtcSimulation::getParticles() {
    return particleMesh_.getObjects();
}

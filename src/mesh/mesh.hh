#pragma once

#include "object/renderable.hh"
#include "object/object.hh"
#include <GL/gl.h>
#include "material/material.hh"
#include "mesh/submesh.hh"
#include "object/camera.hh"
#include "object/object.hh"
#include <GL/gl.h>
#include <glm/fwd.hpp>
#include <string>
#include <vector>

using namespace glm;

class Mesh : public Renderable {
  public:
    static Mesh generateSphere(int slices, int stacks);
    static Mesh generate2DRect(float h, float w, vec3 axis1 = vec3(1,0,0), vec3 axis2 = vec3(0,1,0));
    static Mesh createFrom(std::string path);
    void render(Render &shader, Camera &camera) override;
    const unsigned int id;

    std::vector<Object> &getObjects();
    std::vector<Material> &getMaterials();
    Object &createObject();
    void clearObjects();

  private:
    SubMesh &createSubMesh();
    std::vector<SubMesh> subMeshes_;
    std::vector<Object> objects_;
    std::vector<Material> materials_;

    Mesh();
};

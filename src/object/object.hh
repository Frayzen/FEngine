#pragma once

#include "object/renderable.hh"
#include "object/transform.hh"
#include <map>
#include <vector>

class Mesh;

class Object {
  public:
    Object(Mesh &m);
    Transform getTransform();
    void setTransform(Transform& t);

    vec3& getVelocity();

    static mat4* getTransforms(Mesh& m);
    static vec3* getVelocities(Mesh& m);

    unsigned int id;

  private:
    Mesh& m_;
    static unsigned int getId(Mesh &m);
    static std::map<unsigned int, unsigned int> objIds_;
    static std::map<unsigned int, std::vector<mat4>> objTransforms_;
    static std::map<unsigned int, std::vector<vec3>> objVelocity_;

};


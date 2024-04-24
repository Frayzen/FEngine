#pragma once

#include "object/renderable.hh"
#include "object/transform.hh"
#include <map>
#include <vector>

class Mesh;

class Object {
  public:
    Object(Mesh &m);
    vec4* getColor();
    Transform getTransform();
    void setTransform(Transform& t);

    vec3& getVelocity();

    static void clearObjects(Mesh& m);

    static mat4* getTransforms(Mesh& m);
    static vec4* getVelocities(Mesh& m);
    static vec4* getColors(Mesh& m);

    unsigned int id;

  private:
    Mesh& m_;
    static unsigned int getId(Mesh &m);
    static std::map<unsigned int, unsigned int> objIds_;
    static std::map<unsigned int, std::vector<mat4>> objTransforms_;
    static std::map<unsigned int, std::vector<vec4>> objVelocity_;
    static std::map<unsigned int, std::vector<vec4>> objColors_;

};


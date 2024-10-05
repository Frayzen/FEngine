#pragma once

#include "object/transform.hh"
#include <glm/fwd.hpp>
#include <map>
#include <vector>

class Mesh;

class Object {
  public:
    Object(Mesh &m);

    Transform getTransform();
    void setTransform(Transform &t);
    void setScale(vec3 scale);
    void setPosition(vec3 pos);
    void setRotation(quat rot);

    vec3 &getVelocity();

    static void clearObjects(Mesh &m);

    static mat4 *getTransforms(Mesh &m);
    static vec4 *getVelocities(Mesh &m);

    unsigned int id;

    Mesh &getMesh();

  private:
    Mesh &m_;
    static unsigned int getId(Mesh &m);
    static std::map<unsigned int, unsigned int> objIds_;
    static std::map<unsigned int, std::vector<mat4>> objTransforms_;
    static std::map<unsigned int, std::vector<vec4>> objVelocity_;
    static std::map<unsigned int, std::vector<vec4>> objColors_;
};

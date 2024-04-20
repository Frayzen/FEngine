#pragma once

#include "object/renderable.hh"
#include "object/transform.hh"

class Mesh;

class Object {
  public:
    Object(Mesh &m, unsigned int id);
    Transform getTransform();
    void setTransform(Transform& t);
  private:
    Mesh& m_;
    unsigned int id_ = 0;
};

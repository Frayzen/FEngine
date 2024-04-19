#pragma once

#include "object/renderable.hh"
#include "object/transform.hh"

class Object {
  public:
    Object(mat4& t);
    mat4& mat;

  public: 
    Transform getTransform();
    void setTransform(Transform& t);
};

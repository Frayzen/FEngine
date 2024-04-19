#include "object.hh"
#include "object/camera.hh"
#include <glm/fwd.hpp>
#include <iostream>
#include <GL/glew.h>

Object::Object() : transform(Transform::identity()) {}

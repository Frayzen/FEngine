#include "object.hh"
#include "mesh/mesh.hh"
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <GL/glew.h>

// Define static member variables
std::map<unsigned int, unsigned int> Object::objIds_;
std::map<unsigned int, std::vector<mat4>> Object::objTransforms_;
std::map<unsigned int, std::vector<vec4>> Object::objVelocity_;


void Object::clearObjects(Mesh& m)
{
    objIds_[m.id] = 0;
    objVelocity_[m.id] = std::vector<vec4>();
    objTransforms_[m.id] = std::vector<mat4>();
}

unsigned int Object::getId(Mesh &m) {
    objIds_.try_emplace(m.id, 0);
    return objIds_[m.id]++;
}

Mesh& Object::getMesh()
{
  return m_;
}

Object::Object(Mesh &m) : id(getId(m)), m_(m) {
    static const mat4 mat = Transform::identity().getMatrix();
    objTransforms_[m.id].push_back(mat);
    objVelocity_[m.id].push_back(vec4(0.0f));
}

Transform Object::getTransform() {
    vec3 pos, skew, scale;
    vec4 persp;
    quat rot;
    glm::decompose(objTransforms_[m_.id][id], scale, rot, pos, skew, persp);
    return Transform(pos, rot, scale);
}

void Object::setTransform(Transform &t) {
    objTransforms_[m_.id][id] = t.getMatrix();
}

mat4 *Object::getTransforms(Mesh &m) {
    if (objTransforms_.size() <= m.id)
        return nullptr;
    return objTransforms_[m.id].data();
}

vec4 *Object::getVelocities(Mesh &m) {
    if (objVelocity_.size() <= m.id)
        return nullptr;
    return objVelocity_[m.id].data();
}


void Object::setScale(vec3 scale)
{
    Transform t = getTransform();
    t.scale = scale;
    setTransform(t);
}

void Object::setPosition(vec3 pos)
{
    Transform t = getTransform();
    t.position = pos;
    setTransform(t);
}

void Object::setRotation(quat rot)
{
    Transform t = getTransform();
    t.rotation = rot;
    setTransform(t);
}

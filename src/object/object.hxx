#pragma once

#include "mesh/mesh.hh"
#include <map>
#include <string>

static std::map<unsigned int, std::map<const std::string, std::vector<char>>>
    objAttributes_;

#define ATBS objAttributes_[m_.id]
template <typename T> T Object::getAttribute(const std::string &name) {
    auto attr = ATBS[name];
    std::vector<T> vec = std::vector<T>(attr.begin(), attr.end());
    return vec[id];
}

template <typename T>
void Object::setAttribute(const std::string &name, T &value) {
    auto attr = objAttributes_[m_.id][name];
    std::vector<T> vec = std::vector<T>(attr.begin(), attr.end());
    vec[id] = value;
}

template <typename T>
void Object::addAttribute(Mesh &m, const std::string &name,
                          const T &defaultValue) {
    if (objAttributes_[m.id].find(name) != objAttributes_[m.id].end())
        return;
    auto vec = std::vector<T>(m.getObjects().size(), defaultValue);
    std::vector<char> charVec(vec.begin(), vec.end());
    objAttributes_[m.id][name] = charVec;
}

#include "fem_2d_mesh.hh"
#include "simulation/fem/fem_consts.hh"
#include <algorithm>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
FEM2DMesh::NodeType FEM2DMesh::parseNodeType(const std::string &str) {
    if (str == "FIXED")
        return NodeType::FIXED;
    if (str == "FREE")
        return NodeType::FREE;
    if (str == "ROLLER_X")
        return NodeType::ROLLER_X;
    throw std::invalid_argument("Unknown NodeType: " + str);
}
bool FEM2DMesh::loadFromCSV(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        // Parse the beam endpoints
        float x1, y1, z1, x2, y2, z2;
        std::getline(ss, token, ',');
        x1 = std::stof(token);
        std::getline(ss, token, ',');
        y1 = std::stof(token);
        std::getline(ss, token, ',');
        z1 = std::stof(token);
        std::getline(ss, token, ',');
        x2 = std::stof(token);
        std::getline(ss, token, ',');
        y2 = std::stof(token);
        std::getline(ss, token, ',');
        z2 = std::stof(token);

        // Parse the node types
        std::string nodeType1Str, nodeType2Str;
        std::getline(ss, nodeType1Str, ',');
        std::getline(ss, nodeType2Str, ',');

        NodeType nodeType1 = parseNodeType(nodeType1Str);
        NodeType nodeType2 = parseNodeType(nodeType2Str);

        // Add the beam
        vec3 v1(x1, y1, z1);
        vec3 v2(x2, y2, z2);
        add_beam(v1, v2);

        // Set node types (for this example assuming the IDs correspond to order
        // of addition)
        setNodeType(elems1d_.size() - 2, nodeType1);
        setNodeType(elems1d_.size() - 1, nodeType2);
    }

    file.close();
    return true;
}
void FEM2DMesh::setNodeType(int id, NodeType type) { nodeTypes_[id] = type; }
FEM2DMesh::FEM2DMesh() : beam_(Mesh::generate2DRect(SQR_SIZE, GAP_RECT)) {
    beam_.getMaterials()[0].setColor(vec3(0.0, 0.0, 1.0), true);
}

int find_or_append(vec3 e, std::vector<vec3> &v) {
    auto found1 = std::find(v.begin(), v.end(), e);
    if (found1 == v.end()) {
        v.push_back(e);
        return v.size() - 1;
    }
    return found1 - v.begin();
}

void FEM2DMesh::add_beam(vec3 v1, vec3 v2) {
    auto o = beam_.createObject();
    o.setScale(vec3(SQR_SIZE, SQR_SIZE, 0));

    int id1 = find_or_append(v1, elems0d_);
    int id2 = find_or_append(v2, elems0d_);

    elems1d_.push_back(uvec2(id1, id2));
    updatePos(elems1d_.size() - 1);
}

void FEM2DMesh::updatePos(int id) {
    Object &o = beam_.getObjects()[id];
    vec3 v1 = elems0d_[elems1d_[id].x];
    vec3 v2 = elems0d_[elems1d_[id].y];

    auto dir = v1 - v2;
    auto len = length(dir);
    auto rot = glm::quat_cast(
        glm::rotate(identity<mat4>(), std::atan2(dir.z, dir.y), vec3(1, 0, 0)));
    Transform t = o.getTransform();
    t.scale = vec3(len, len, 1);
    t.position = (v1 + v2) / 2.0f;
    t.rotation = rot;
    o.setTransform(t);
}

Mesh &FEM2DMesh::getMesh(void) { return beam_; }

#include "constants.hh"
#include "mesh/submesh.hh"
#include "mesh/mesh.hh"
#include "ray.hh"
#include "object/object.hh"
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <limits>
#include <ostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

Ray Ray::fromClick(Camera &cam, vec2 &where) {
    (void)cam;
    (void)where;
    Ray ray(cam.position, cam.getFront());
    return ray;
}

Ray::Ray(vec3 org, vec3 dir) : org(org), dir(normalize(dir)) {
    dirfrac = 1.0f / dir;
}

bool Ray::intersect(Object &o, vec3 &out) {
    Mesh &m = o.getMesh();
    vec3 found(0);
    float dist = MAXF;
    for (SubMesh &sm : m.getSubMeshes()) {
        auto pos = o.getTransform().position;
        if (!sm.getAABbox().intersect(*this, pos))
            continue;
        for (unsigned int i = 0; i < sm.triangleNumber(); i++) {
            auto m = sm.getTriangle(i);
            m[0] += pos;
            m[1] += pos;
            m[2] += pos;
            vec3 bary;
            if (!glm::intersectLineTriangle(org, dir, m[0], m[1], m[2],
                                            bary))
                continue;
            auto position = (1 - bary.y - bary.z) * m[0] + bary.y * m[1] + bary.z * m[2];
            auto curdist = distance(position, org);
            if (dist <= curdist)
                continue;
            found = position;
            dist = curdist;
        }
    }
    if (dist == MAXF)
        return false;
    out = found;
    return true;
}

bool Ray::intersect(std::vector<Object> &objs, int &outId, vec3 &pos) {
    float dist = MAXF;
    vec3 out;
    for (unsigned int id = 0; id < objs.size(); id++) {
        Object &o = objs[id];
        if (intersect(o, out)) {
            auto curr = distance(out, org);
            if (curr < dist) {
                outId = id;
                dist = curr;
                pos = out;
            }
        }
    }
    return dist != MAXF;
}

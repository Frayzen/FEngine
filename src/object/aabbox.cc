#include "aabbox.hh"
#include "constants.hh"
#include <glm/common.hpp>

AABBox AABBox::noBounds(void) {

    return AABBox(vec3(MAXF, MAXF, MAXF), vec3(MINF, MINF, MINF));
}

AABBox::AABBox(vec3 min, vec3 max) : min_(min), max_(max) {}

bool AABBox::intersect(Ray &r, vec3 offset) {
    vec3 mn = min_ + offset;
    vec3 mx = max_ + offset;
    // min_ is the corner of AABB with minimal coordinates - left bottom, max_
    // is maximal corner r.org is origin of ray
    float t1 = (mn.x - r.org.x) * r.dirfrac.x;
    float t2 = (mx.x - r.org.x) * r.dirfrac.x;
    float t3 = (mn.y - r.org.y) * r.dirfrac.y;
    float t4 = (mx.y - r.org.y) * r.dirfrac.y;
    float t5 = (mn.z - r.org.z) * r.dirfrac.z;
    float t6 = (mx.z - r.org.z) * r.dirfrac.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is
    // behind us
    if (tmax < 0)
        return false;
    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
        return false;
    return true;
}

void AABBox::update(const vec3 &v) {
    min_ = min(v, min_);
    max_ = max(v, max_);
}

#include "mesh.hh"
#include "mesh/submesh.hh"

#define M_PI 3.14159265358979323846

Mesh Mesh::generate2DRect(float h, float w) {
    Mesh mesh = Mesh();
    mesh.materials_.emplace_back(Material());
    SubMesh &sm = mesh.createSubMesh();

    // add vertices
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            auto pos = vec3(0, i * h, j * w);
            auto norm = vec3(1, 0, 0);
            auto uv = vec3(i, j, 0);
            sm.addVertex(pos, norm, uv);
        }
    }

    // add triangles
    sm.addTriangle(uvec3(0, 1, 2));
    sm.addTriangle(uvec3(1, 2, 3));

    return mesh;
}

Mesh Mesh::generateSphere(int n_slices, int n_stacks) {
    Mesh mesh = Mesh();
    mesh.materials_.emplace_back(Material());
    SubMesh &sm = mesh.createSubMesh();

    // add top vertex
    auto pos = vec3(0, 1, 0);
    auto v0 = sm.addVertex(pos, pos, pos);

    // generate vertices per stack / slice
    for (int i = 0; i < n_stacks - 1; i++) {
        auto phi = M_PI * double(i + 1) / double(n_stacks);
        for (int j = 0; j < n_slices; j++) {
            auto theta = 2.0 * M_PI * double(j) / double(n_slices);
            auto x = std::sin(phi) * std::cos(theta);
            auto y = std::cos(phi);
            auto z = std::sin(phi) * std::sin(theta);
            vec3 pos = vec3(x, y, z);
            sm.addVertex(pos, pos, pos);
        }
    }

    // add bottom vertex
    pos = vec3(0, -1, 0);
    auto v1 = sm.addVertex(pos, pos, pos);

    // add top / bottom triangles
    for (int i = 0; i < n_slices; ++i) {
        auto i0 = i + 1;
        auto i1 = (i + 1) % n_slices + 1;
        sm.addTriangle(uvec3(v0, i1, i0));
        i0 = i + n_slices * (n_stacks - 2) + 1;
        i1 = (i + 1) % n_slices + n_slices * (n_stacks - 2) + 1;
        sm.addTriangle(uvec3(v1, i0, i1));
    }

    // add quads per stack / slice
    for (int j = 0; j < n_stacks - 2; j++) {
        auto j0 = j * n_slices + 1;
        auto j1 = (j + 1) * n_slices + 1;
        for (int i = 0; i < n_slices; i++) {
            auto i0 = j0 + i;
            auto i1 = j0 + (i + 1) % n_slices;
            auto i2 = j1 + (i + 1) % n_slices;
            auto i3 = j1 + i;
            sm.addTriangle(uvec3(i0, i1, i2));
            sm.addTriangle(uvec3(i0, i2, i3));
        }
    }
    return mesh;
}

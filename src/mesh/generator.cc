#include "mesh.hh"

#define M_PI 3.14159265358979323846

Mesh Mesh::generateSphere(int n_slices, int n_stacks) {
    Mesh mesh = Mesh();

    // add top vertex
    auto v0 = mesh.addVertex(vec3(0, 1, 0));

    // generate vertices per stack / slice
    for (int i = 0; i < n_stacks - 1; i++) {
        auto phi = M_PI * double(i + 1) / double(n_stacks);
        for (int j = 0; j < n_slices; j++) {
            auto theta = 2.0 * M_PI * double(j) / double(n_slices);
            auto x = std::sin(phi) * std::cos(theta);
            auto y = std::cos(phi);
            auto z = std::sin(phi) * std::sin(theta);
            mesh.addVertex(vec3(x, y, z));
        }
    }

    // add bottom vertex
    auto v1 = mesh.addVertex(vec3(0, -1, 0));

    // add top / bottom triangles
    for (int i = 0; i < n_slices; ++i) {
        auto i0 = i + 1;
        auto i1 = (i + 1) % n_slices + 1;
        mesh.addTriangle(uvec3(v0, i1, i0));
        i0 = i + n_slices * (n_stacks - 2) + 1;
        i1 = (i + 1) % n_slices + n_slices * (n_stacks - 2) + 1;
        mesh.addTriangle(uvec3(v1, i0, i1));
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
            mesh.addTriangle(uvec3(i0, i1, i2));
            mesh.addTriangle(uvec3(i0, i2, i3));
        }
    }
    return mesh;
}

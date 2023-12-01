
#include "renderer/mesh/mesh.h"
#include "renderer/renderer.h"
int main(void)
{
    renderer *rd = initRenderer();
    defineVertexShader(rd, "./assets/vertexShader.glsl");
    defineFragmentShader(rd, "./assets/fragmentShadder.glsl");
    mesh *m = createMeshFromObj("./assets/pyramid.obj");
    addMesh(rd, m);
    // printMesh(m);
    startRendering(rd);
    destroyRenderer(rd);
    return 0;
}

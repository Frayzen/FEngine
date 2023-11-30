
#include "renderer/renderer.h"
int main(void)
{
    renderer *rd = initRenderer("FEngine");
    defineVertexShader(rd, "./assets/vertexShader.glsl");
    defineFragmentShader(rd, "./assets/fragmentShadder.glsl");
    startRendering(rd);
    destroyRenderer(rd);
    return 0;
}

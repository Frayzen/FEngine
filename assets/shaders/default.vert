#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aUv;
layout (location = 3) in mat4 objTransform;
layout (location = 8) in vec4 aColor;
uniform mat4 camMat;
out vec4 pos;
out vec3 uv;
out vec4 color;
out vec3 norm;
void main()
{
    mat4 mat = camMat * objTransform;
    pos = mat * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    norm = aNorm;
    color = aColor;
    uv = aUv;
    gl_Position = pos;
}

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
uniform mat4 transformMat;
out vec4 pos;
out vec3 norm;
void main()
{
    pos = transformMat * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    norm = aNorm;
    gl_Position = pos;
}

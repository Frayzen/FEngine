#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transformMat;
out vec4 pos;
void main()
{
    pos = transformMat * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    gl_Position = pos;
}

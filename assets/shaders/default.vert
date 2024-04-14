#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 camMat;
out vec4 pos;
void main()
{
    pos = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    pos *= camMat;
    gl_Position = pos;
}

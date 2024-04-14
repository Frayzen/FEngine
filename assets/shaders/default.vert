#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 camMat;
void main()
{
    vec4 pos = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    pos *= camMat;
    gl_Position = pos;
}

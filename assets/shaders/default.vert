#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aUv;
layout (location = 3) in mat4 objTransform;
layout (location = 8) in vec4 aColor;
uniform mat4 camMat;
out vec3 uv;
out vec3 wordPos;
out vec4 color;
out vec3 norm;
void main()
{
    wordPos = vec3(objTransform * vec4(aPos.xyz, 1.0f));
    mat4 mat = camMat * objTransform;
    norm = vec3(normalize(objTransform * vec4(aNorm, 0.0f)));
    color = aColor;
    uv = aUv;
    gl_Position = camMat * vec4(wordPos, 1.0f);
}

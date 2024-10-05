#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aUv;
layout (location = 8) in vec4 aColor;

layout (location = 3) in mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 camPos;

out vec3 uv;
out vec3 wordPos;
out vec4 color;
out vec3 norm;
void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    norm = normalize(mat3(model) * aNorm);
    color = aColor;
    uv = aUv;
    wordPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

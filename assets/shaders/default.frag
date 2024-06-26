#version 330 core
out vec4 result;
in vec4 pos;
in vec3 norm;
in vec4 color;

uniform vec3 lightPos;

void main()
{
    vec3 lightDir = normalize(lightPos - pos.xyz);
    float diffuse  = 1.0f;
    if (length(norm) > 0.001f)
        diffuse = max(dot(normalize(norm), lightDir), 0);
    result = color /** diffuse*/;
}

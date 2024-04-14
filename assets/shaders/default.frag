#version 330 core
out vec4 color;
in vec4 pos;
void main()
{
    float d = length(pos);
    color = vec4(d, d, d, 1.0f);
}

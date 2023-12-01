#version 330 core
out vec4 FragColor;

in vec3 normal;

void main()
{
    vec3 lightSource = vec3(0.0f, 1.0f, 0.0f);
    float intensity = dot(normal, lightSource);
    FragColor = vec4(intensity, intensity, intensity, 1.0f);
}

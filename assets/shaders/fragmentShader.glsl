#version 330 core
out vec4 FragColor;

in vec3 normal;

uniform vec3 camera;

void main()
{
    vec3 lightSource = vec3(0.0f, 1.0f, 0.0f);
    float intensity = clamp(dot(normal, lightSource), 0., 1.);
    FragColor = vec4(intensity, intensity, intensity, 1.0f);
}

#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec2 texCoord;

uniform vec3 camera;
uniform sampler2D texture1;

void main()
{
    vec3 lightSource = vec3(0.0f, 1.0f, 0.0f);
    float intensity = clamp(dot(normal, lightSource), 0., 1.);
    vec3 color = texture(texture1, texCoord).rgb;
    FragColor = vec4(intensity, intensity, intensity, 1.0f) * vec4(color, 1.0f);
}

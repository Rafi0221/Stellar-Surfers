#version 330 core
out vec4 FragColor;

uniform vec3 seed;

in vec3 cPos;

void main()
{
    FragColor = vec4(seed, 1.0f);
}

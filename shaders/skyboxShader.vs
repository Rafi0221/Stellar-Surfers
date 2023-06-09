#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 quadPos;

out vec3 cPos;

void main()
{
    gl_Position = vec4(quadPos, 1.0);
    cPos = aPos;
}

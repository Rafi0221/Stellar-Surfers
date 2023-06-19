#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexPos;
layout (location = 2) in float aType;

out vec2 bTexPos;
flat out float bType;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    bTexPos = aTexPos;
    bType = aType;
}

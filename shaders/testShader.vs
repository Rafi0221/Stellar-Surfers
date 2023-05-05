#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 Pos;

void main()
{
    Pos = aPos;
//    height = (aPos.y + 0.75f) / (2 * 0.75f);
    gl_Position = vec4(aPos, 1.0);
}

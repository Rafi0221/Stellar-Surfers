#version 330 core
out vec4 FragColor;

in float bDist;

void main()
{
    float x = 1.0 - abs(bDist);
    x = (x * 2.0) - 1.0;
    FragColor = vec4(0.27, 0.95, 0.15, x);
}

#version 330 core
out vec4 FragColor;

in vec2 bRelPos;

void main()
{
    float dist = bRelPos.x * bRelPos.x + bRelPos.y * bRelPos.y;
    float x = 1.0 - dist;
//    x = (x * 2.0);
    FragColor = vec4(0.27, 0.95, 0.15, x);
}

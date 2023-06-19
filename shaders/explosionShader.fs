#version 330 core
out vec4 FragColor;

in vec2 bTexPos;
flat in float bType;

uniform sampler2D texture1;
uniform float fade;

void main()
{
    float transparency = texture(texture1, bTexPos).r;
    vec3 color;
//    int bType = 2;
    if(bType == 0){
        float x = bTexPos.x * 8.0 - 1.0;
        float y = bTexPos.y * 8.0 - 1.0;
        float dist = x * x + y * y;
        float f = 1.0 - dist;
        f *= fade;
        color = mix(vec3(1,1,0),vec3(1,0.5,0), 1-f);
        transparency = f;
    }else if(bType == 1){
        color = vec3(transparency * 230.0/255.0, transparency * 30/255.0, transparency * 0/255.0);
        color *= fade;
        transparency *= fade;
    }else if(bType == 2){
        color = mix(vec3(1,1,0.9),vec3(1.0,0.8,0.09), 1.0-transparency);
        // color = mix(vec3(1,1,0), vec3(1,0.5,0), fade);
        color *= fade;
        transparency *= fade;
    }else if(bType == 3){
        color = mix(vec3(253.0/255.0, 250.0/255.0, 224.0/255.0), vec3(89.0/255.0, 71.0/255.0, 49.0/255.0), 1-transparency);
        color *= fade;
        transparency *= fade;
    }
    FragColor = vec4(color, transparency);
}

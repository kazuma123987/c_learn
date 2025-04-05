#version 400 core
in vec2 texCoord;
uniform sampler2D image;
uniform vec4 spriteColor;
uniform float darker;
out vec4 FragColor;
void main()
{
    FragColor=spriteColor*texture(image,texCoord);
    FragColor.rgb=pow(FragColor.rgb,vec3(darker));
}
#version 450
layout(location = 0) in vec4 fragColor;
layout(location = 0) out vec4 outFragColor;

void main()
{
    outFragColor = fragColor;
}
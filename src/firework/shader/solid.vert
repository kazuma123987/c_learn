#version 450
layout(location = 0) in vec2 inPosition; // 圆形顶点位置
layout(location = 1) in vec2 inOffset;   // 实例化偏移
layout(location = 2) in float inScale;   // 实例化缩放
layout(location = 3) in vec4 inColor;    // 实例化颜色
layout(location = 0) out vec4 fragColor;

uniform mat4 proj;

void main()
{
    vec2 position = inPosition * inScale + inOffset;
    gl_Position = proj * vec4(position, 0.0, 1.0);
    fragColor = inColor;
}
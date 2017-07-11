#version 410 core
layout (location=0) in vec2 vPosition;
void main()
{
    gl_Position = vec4(vPosition.xy, 0, 1);
}
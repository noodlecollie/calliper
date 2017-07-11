#version 410 core

layout (location=0) in vec2 vPosition;

out vec2 fPosition;

void main()
{
    gl_Position = vec4(vPosition.xy, 0, 1);
    fPosition = vPosition;
}

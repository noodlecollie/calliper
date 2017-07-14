#version 410 core

layout (location=0) in vec2 vPosition;

out vec2 fTexCoord;

void main()
{
   gl_Position = vec4(vPosition.xy, 0, 1);
   fTexCoord = vec2(vPosition.x + 1, vPosition.y + 1) / 2.0;
}
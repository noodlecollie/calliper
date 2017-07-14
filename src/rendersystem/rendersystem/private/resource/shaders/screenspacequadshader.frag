#version 410 core

in vec2 fTexCoord;

uniform sampler2D tex;

layout(location = 0) out vec4 color;

void main()
{
   color = texture(tex, fTexCoord);
}
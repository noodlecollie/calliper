#version 410 core
in vec4 fColour;
in vec2 fTexCoord;
layout(location = 0) out vec4 color;
uniform sampler2D tex;
void main()
{
   color = texture(tex, fTexCoord) /* * fColour*/;
}
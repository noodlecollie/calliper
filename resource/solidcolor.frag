#version 330 core
layout(location = 0) uniform mediump vec4 vec_Color;  // Colour for all fragments
out vec4 vecColor;
void main()
{
    vecColor = vec_Color;
}

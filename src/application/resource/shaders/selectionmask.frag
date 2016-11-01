#version 410 core

out vec4 colour;

uniform vec4 inCol;
in vec3 fVColor;

void main()
{
    if ( inCol == vec4(1,1,1,1) )
    {
        colour = vec4(fVColor, 1);
    }
    else
    {
        colour = inCol;
    }
}

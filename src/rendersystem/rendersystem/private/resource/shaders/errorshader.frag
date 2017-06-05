#version 410 core

// Outputs
layout(location = 0) out vec4 color;

bool state(int val)
{
    return val % 8 < 3;
}

void main()
{
    if ( state(gl_FragCoord.x) == state(gl_FragCoord.y) )
    {
	color = vec4(1, 0.322, 0.875, 1);
    }
    else
    {
        color = vec4(0, 0, 0, 1);
    }
}

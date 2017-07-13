// Outputs
layout(location = 0) out vec4 color;

bool state(int val)
{
    const int delta = 16;
    return val % delta < (delta/2);
}

void main()
{
    if ( state(int(gl_FragCoord.x)) == state(int(gl_FragCoord.y)) )
    {
        color = vec4(1, 0.322, 0.875, 1);
    }
    else
    {
        color = vec4(0, 0, 0, 1);
    }
}

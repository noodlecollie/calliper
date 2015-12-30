#version 410 core

in vec4 fColor;
in vec3 fViewSpace;

out vec4 color;

uniform vec4 fFogColor;
uniform float fFogBegin;
uniform float fFogEnd;

void main()
{
        float dist = abs(fViewSpace.z);
        float fogFactor = 0;
        if ( fFogEnd != fFogBegin )
        {
            fogFactor = clamp((dist-fFogBegin)/(fFogEnd-fFogBegin), 0, 1);
        }

        color = mix(fColor, fFogColor, fogFactor);
}

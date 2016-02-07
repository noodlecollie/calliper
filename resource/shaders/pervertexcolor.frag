#version 410 core

in vec4 fVColor;
in vec3 fViewSpace;
in vec3 fNormal;

out vec4 color;

uniform vec4 fColor;
uniform vec4 fFogColor;
uniform float fFogBegin;
uniform float fFogEnd;
uniform vec3 directionalLight;

void main()
{
    vec4 col = fVColor * fColor;

    // Get the dot product of the normal with the light.
    // We rescale this to make sure that the faces don't get too dark.
    // The rescaling is [0.5 1]
    float dp = dot(fNormal, directionalLight);
    dp = 0.5 + (0.5 * ((0.5*dp) + 0.5));

    float dist = abs(fViewSpace.z);
    float fogFactor = 0;
    if ( fFogEnd != fFogBegin )
    {
        fogFactor = clamp((dist-fFogBegin)/(fFogEnd-fFogBegin), 0, 1);
    }

    color = mix(vec4(dp * col.xyz, col.w), fFogColor, fogFactor);
}

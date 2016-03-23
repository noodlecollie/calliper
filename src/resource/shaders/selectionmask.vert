#version 410 core

// Incoming attributes.
in vec3 vPositionModelSpace;
in vec4 vColor;

out vec3 fVColor;

// Uniforms
uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 hammerToOpenGL;
uniform mat4 projection;
uniform float counterScale;

void main()
{
        gl_Position = projection * hammerToOpenGL * worldToCamera * modelToWorld * vec4(counterScale * vPositionModelSpace, 1);
        fVColor = vColor.rgb;
}

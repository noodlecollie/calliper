#version 410 core

// Incoming attributes
in vec3 vPositionModelSpace;
in vec4 vColor;

// Attributes to be passed on to the fragment shader.
out vec4 fColor;
out vec3 fViewSpace;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 hammerToOpenGL;
uniform mat4 projection;
uniform float counterScale;

void main()
{
        vec4 intermediate = hammerToOpenGL * worldToCamera * modelToWorld * vec4(counterScale * vPositionModelSpace, 1);
        fViewSpace = intermediate.xyz;
        gl_Position = projection * intermediate;

	fColor = vColor;
}

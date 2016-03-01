#version 410 core

// Incoming attributes
in vec3 vPositionModelSpace;
in vec3 vNormal;
in vec4 vColor;

// Attributes to be passed on to the fragment shader.
out vec4 fVColor;
out vec3 fViewSpace;
out vec3 fNormal;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 hammerToOpenGL;
uniform mat4 projection;
uniform float counterScale;
uniform vec3 directionalLight;

void main()
{
        vec4 intermediate = hammerToOpenGL * worldToCamera * modelToWorld * vec4(counterScale * vPositionModelSpace, 1);
        fViewSpace = intermediate.xyz;
        gl_Position = projection * intermediate;

        fVColor = vColor;

        // Convert the normals to world space.
        // If the normal is 0 (ie. not applicable), pass it as the opposite direction to the directional light.
        // This means the vertex will be at full brightness.
        fNormal = length(vNormal) == 0 ? -directionalLight : normalize((modelToWorld * vec4(vNormal, 0)).xyz);
}

#version 410 core

// Incoming attributes.
in vec3 vPositionModelSpace;
in vec3 vNormal;
in vec2 vUV;

// Outgoing attributes to the fragment shader.
out vec2 fUV;
out vec3 fNormal;
out vec3 fViewSpace;

// Uniforms
uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 hammerToOpenGL;
uniform mat4 projection;
uniform vec3 directionalLight;

bool isNull(vec3 v)
{
    const float EPS = 0.05;
    return v.x < EPS && v.y < EPS && v.z < EPS;
}

void main()
{
        vec4 intermediate = hammerToOpenGL * worldToCamera * modelToWorld * vec4(vPositionModelSpace, 1);
        fViewSpace = intermediate.xyz;
        gl_Position = projection * intermediate;

	// Pass on the rest of the attributes.

	// Nothing needs to be done to the UVs.
	fUV = vUV;

	// Convert the normals to world space.
        // If the normal is 0 (ie. not applicable), pass it as the opposite direction to the directional light.
        // This means the vertex will be at full brightness.
        fNormal = isNull(vNormal) ? -directionalLight : normalize((modelToWorld * vec4(vNormal, 0)).xyz);
}

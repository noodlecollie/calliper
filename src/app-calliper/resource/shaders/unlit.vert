#version 410 core

// Incoming attributes.
in vec3 vPositionModelSpace;
in vec2 vUV;

// Outgoing attributes to the fragment shader.
out vec2 fUV;

// Uniforms
uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 hammerToOpenGL;
uniform mat4 projection;
uniform float counterScale;

void main()
{
	mat4 mat = projection * hammerToOpenGL * worldToCamera * modelToWorld;
        gl_Position = mat * vec4(counterScale * vPositionModelSpace, 1);

	// Pass on the rest of the attributes.

	// Nothing needs to be done to the UVs.
	fUV = vUV;
}

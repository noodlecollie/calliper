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

void main()
{
		mat4 mv = hammerToOpenGL * worldToCamera * modelToWorld;
		float sc = (projection * mv * vec4(0,0,0,1)).w;

        vec4 intermediate = mv * vec4(vPositionModelSpace * sc, 1);
        fViewSpace = intermediate.xyz;
        gl_Position = projection * intermediate;

	fColor = vColor;
}

#version 410 core

// Incoming attributes
in vec3 vPositionModelSpace;
in vec4 vColor;

// Attributes to be passed on to the fragment shader.
out vec4 fColor;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 hammerToOpenGL;
uniform mat4 projection;

void main()
{
        mat4 mat = projection * hammerToOpenGL * worldToCamera * modelToWorld;
        gl_Position = mat * vec4(vPositionModelSpace, 1);

	fColor = vColor;
}

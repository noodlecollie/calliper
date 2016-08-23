#version 410 core
in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;
out vec4 fVColor;
out vec3 fNormal;
uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 hammerToOpenGL;
uniform mat4 projection;
void main()
{
        gl_Position = projection * hammerToOpenGL * worldToCamera * modelToWorld * vec4(vPosition, 1);
        fVColor = vColor;
        fNormal = normalize((modelToWorld * vec4(vNormal, 0)).xyz);
}

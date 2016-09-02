#version 410 core
layout (location=0) in vec4 vPosition;
layout (location=2) in vec4 vColour;
layout (location=3) in vec2 vTexCoord;
layout (std140) uniform LocalUniformBlock
{
   mat4 modelToWorldMatrices[8];
};
out vec4 fColour;
out vec2 fTexCoord;
void main()
{
   uint id = uint(vPosition.z);
   gl_Position = modelToWorldMatrices[id] * vec4(vPosition.xy, 0, 1);
   fColour = vColour;
   fTexCoord = vTexCoord;
}
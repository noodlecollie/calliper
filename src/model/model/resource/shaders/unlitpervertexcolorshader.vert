// Global uniforms etc. are imported in common headers

// Input attributes
layout (location=0) in vec4 vPosition;
layout (location=2) in vec4 vColour;

// Local uniforms
layout (std140) uniform LocalUniformBlock
{
   mat4 modelToWorldMatrices[8];
};

// Outputs to fragment shaders
out vec4 fColour;

void main()
{
        // Pull the object ID out of w.
        uint id = uint(vPosition.w);

        gl_Position = projectionMatrix * COORD_TRANSFORM_HAMMER_OPENGL
                *  worldToCameraMatrix * modelToWorldMatrices[id] * vec4(vPosition.xyz, 1);

        fColour = vColour;
}
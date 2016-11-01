// Global uniforms etc. are imported in common headers

// Input attributes
layout (location=0) in vec4 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec4 vColour;
layout (location=3) in vec2 vTexCoord;

// Local uniforms
layout (std140) uniform LocalUniformBlock
{
   mat4 modelToWorldMatrices[8];
};

// Outputs to fragment shaders
out vec4 fColour;
out vec3 fNormal;
out vec2 fTexCoord;

void main()
{
        // Pull the object ID out of w.
        uint id = uint(vPosition.w);

        // Position gets the entire transform.
        gl_Position = projectionMatrix * COORD_TRANSFORM_HAMMER_OPENGL * worldToCameraMatrix
        	* modelToWorldMatrices[id] * vec4(vPosition.xyz, 1);

        // Normals stay in world co-ords.
        fNormal = normalize( (modelToWorldMatrices[id] * vec4(vNormal, 0)).xyz );
        if ( length(fNormal) != 0.0 )
        {
        	fNormal = normalize(fNormal);
        }

        fColour = vColour;
        fTexCoord = vTexCoord;
}
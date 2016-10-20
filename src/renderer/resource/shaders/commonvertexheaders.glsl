#version 410 core

// OpenGL and Hammer have the following differences to take into account:
// 1. Co-ordinate systems:
//    - OpenGL is right-handed, with X pointing right, Y pointing up and Z pointing out of the screen.
//    - Hammer is left-handed, with X pointing right, Y pointing into the screen and Z pointing up.
// 2. Default orientations:
//    - OpenGL places its camera pointing down the negative Z axis (as per the above system), with
//      positive Y pointing up.
//    - Hammer's default orientation for objects with angles of (0,0,0) is to point down the positive
//      X axis, with positive Z pointing up.
// In order to abstract away from OpenGL implementation details in code, the Hammer co-ordinate system
// is transformed into the OpenGL system in the shaders.
// The co-ordinate transform matrix specified below should be applied after the world-to-camera matrix,
// but before the projection matrix. It is constructed to adjust the camera rotation for the purposes of
// facing down the X axis, and then to transform the entire co-ordinate system into OpenGL space.
// Camera adjustment: [0 -1 0 0]
// (270 deg Z rot)^-1 [1  0 0 0]
//                    [0  0 1 0]
//                    [0  0 0 1]
//
// Hammer to OpenGL:  [1  0 0 0]
//                    [0  0 1 0]
//                    [0 -1 0 0]
//                    [0  0 0 1]
//
// GLSL matrices are constructed in COLUMN MAJOR ORDER.
const mat4 COORD_TRANSFORM_HAMMER_OPENGL =
	mat4(vec4(0,0,-1,0), vec4(-1,0,0,0), vec4(0,1,0,0), vec4(0,0,0,1));

// Global uniforms
layout (std140) uniform GlobalUniformBlock
{
    mat4 worldToCameraMatrix;
    mat4 projectionMatrix;
};

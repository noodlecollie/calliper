#ifndef CALLIPERGLOBALS_H
#define CALLIPERGLOBALS_H

#define PR_VECTOR3 "[%f %f %f]"
#define FM_VECTOR3(v) v.x, v.y, v.z

#define PR_MATRIX4 "[%f %f %f %f]\n[%f %f %f %f]\n[%f %f %f %f]\n[%f %f %f %f]\n"
#define FM_MATRIX4(m) \
m[0][0], m[0][1], m[0][2], m[0][3], \
m[1][0], m[1][1], m[1][2], m[1][3], \
m[2][0], m[2][1], m[2][2], m[2][3], \
m[3][0], m[3][1], m[3][2], m[3][3]

#endif	// CALLIPERGLOBALS_H
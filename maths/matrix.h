#ifndef MATRIX_H
#define MATRIX_H

#include "maths/utils.h"
#include "vector.h"

typedef struct mat4{
    float m[4][4];
} mat4;

typedef struct mat3{
    float m[3][3];
} mat3;

mat4 mat4Identity(void);
mat4 mat4Mul(mat4 a, mat4 b);
mat4 mat4Translate(mat4 a, vec3 b);
mat4 mat4Scale(mat4 a, vec3 b);
mat4 mat4RotateX(mat4 a, float b);
mat4 mat4RotateY(mat4 a, float b);
mat4 mat4RotateZ(mat4 a, float b);
mat4 mat4Ortho(float left, float right, float bottom, float top, float near, float far);
mat4 mat4Perspective(float fov, float aspect, float near, float far);
mat4 mat4Look_at(vec3 eye, vec3 center, vec3 up);
mat4 mat4Transform(transform t);
mat4 mat4View(vec3 pos, vec3 rot);
mat4 mat4Inverse(mat4 m);

vec3 mat4MulVec3(mat4 m, vec3 v);

mat3 mat3Identity(void);
mat3 mat3Mul(mat3 a, mat3 b);
mat3 mat3Translate(mat3 a, vec2 b);
mat3 mat3Scale(mat3 a, vec2 b);
mat3 mat3Rotate(float b);

#endif /* !MATRIX_H */

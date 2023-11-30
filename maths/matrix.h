#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct mat4{
    float m[4][4];
} mat4;

typedef struct mat3{
    float m[3][3];
} mat3;

mat4 mat4_identity(void);
mat4 mat4_mul(mat4 a, mat4 b);
mat4 mat4_translate(mat4 a, vec3 b);
mat4 mat4_scale(mat4 a, vec3 b);
mat4 mat4_rotate_x(mat4 a, float b);
mat4 mat4_rotate_y(mat4 a, float b);
mat4 mat4_rotate_z(mat4 a, float b);
mat4 mat4_ortho(float left, float right, float bottom, float top, float near, float far);
mat4 mat4_perspective(float fov, float aspect, float near, float far);
mat4 mat4_look_at(vec3 eye, vec3 center, vec3 up);
mat4 mat4_transform(vec3 pos, vec3 scale, vec3 rot);

mat3 mat3_identity(void);
mat3 mat3_mul(mat3 a, mat3 b);
mat3 mat3_translate(mat3 a, vec2 b);
mat3 mat3_scale(mat3 a, vec2 b);
mat3 mat3_rotate(float b);

#endif /* !MATRIX_H */

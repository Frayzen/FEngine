#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

struct mat4{
    float m[4][4];
};

struct mat3{
    float m[3][3];
};

struct mat4 mat4_identity(void);
struct mat4 mat4_mul(struct mat4 a, struct mat4 b);
struct mat4 mat4_translate(struct mat4 a, struct vec3 b);
struct mat4 mat4_scale(struct mat4 a, struct vec3 b);
struct mat4 mat4_rotate_x(struct mat4 a, float b);
struct mat4 mat4_rotate_y(struct mat4 a, float b);
struct mat4 mat4_rotate_z(struct mat4 a, float b);
struct mat4 mat4_ortho(float left, float right, float bottom, float top, float near, float far);
struct mat4 mat4_perspective(float fov, float aspect, float near, float far);
struct mat4 mat4_look_at(struct vec3 eye, struct vec3 center, struct vec3 up);

struct mat3 mat3_identity(void);
struct mat3 mat3_mul(struct mat3 a, struct mat3 b);
struct mat3 mat3_translate(struct mat3 a, struct vec2 b);
struct mat3 mat3_scale(struct mat3 a, struct vec2 b);
struct mat3 mat3_rotate(float b);

#endif /* !MATRIX_H */

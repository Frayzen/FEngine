#ifndef VECTORS_H
#define VECTORS_H

typedef struct vec4{
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct vec3{
    float x;
    float y;
    float z;
} vec3;

typedef struct vec2{
    float x;
    float y;
} vec2;

#define VEC4(X, Y, Z, W) ((vec4) {X,Y,Z,W})
vec4 vec4_add(vec4 a, vec4 b);
vec4 vec4_sub(vec4 a, vec4 b);
vec4 vec4_mul(vec4 a, vec4 b);
vec4 vec4_div(vec4 a, vec4 b);
vec4 vec4_scale(vec4 a, float b);
vec4 vec4_cross(vec4 a, vec4 b);
float vec4_dot(vec4 a, vec4 b);
float vec4_length(vec4 a);
vec4 vec4_normalize(vec4 a);

#define VEC3(X, Y, Z) ((vec3) {X,Y,Z})
vec3 vec3_create(float x, float y, float z);
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, vec3 b);
vec3 vec3_div(vec3 a, vec3 b);
vec3 vec3_scale(vec3 a, float b);
vec3 vec3_cross(vec3 a, vec3 b);
float vec3_dot(vec3 a, vec3 b);
float vec3_length(vec3 a);
vec3 vec3_normalize(vec3 a);

#define VEC2(X, Y) ((vec2) {X,Y})
vec3 vec3_create(float x, float y, float z);
vec2 vec2_create(float x, float y);
vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_mul(vec2 a, vec2 b);
vec2 vec2_div(vec2 a, vec2 b);
vec2 vec2_scale(vec2 a, float b);
float vec2_dot(vec2 a, vec2 b);
float vec2_length(vec2 a);
vec2 vec2_normalize(vec2 a);

#endif /* !VECTORS_H */

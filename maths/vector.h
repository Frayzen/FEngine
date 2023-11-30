#ifndef VECTORS_H
#define VECTORS_H

struct vec4{
    float x;
    float y;
    float z;
    float w;
};

struct vec3{
    float x;
    float y;
    float z;
};

struct vec2{
    float x;
    float y;
    float z;
};

struct vec4 vec4_add(struct vec4 a, struct vec4 b);
struct vec4 vec4_sub(struct vec4 a, struct vec4 b);
struct vec4 vec4_mul(struct vec4 a, struct vec4 b);
struct vec4 vec4_div(struct vec4 a, struct vec4 b);
struct vec4 vec4_scale(struct vec4 a, float b);
struct vec4 vec4_cross(struct vec4 a, struct vec4 b);
float vec4_dot(struct vec4 a, struct vec4 b);
float vec4_length(struct vec4 a);
struct vec4 vec4_normalize(struct vec4 a);

struct vec3 vec3_add(struct vec3 a, struct vec3 b);
struct vec3 vec3_sub(struct vec3 a, struct vec3 b);
struct vec3 vec3_mul(struct vec3 a, struct vec3 b);
struct vec3 vec3_div(struct vec3 a, struct vec3 b);
struct vec3 vec3_scale(struct vec3 a, float b);
struct vec3 vec3_cross(struct vec3 a, struct vec3 b);
float vec3_dot(struct vec3 a, struct vec3 b);
float vec3_length(struct vec3 a);
struct vec3 vec3_normalize(struct vec3 a);

struct vec2 vec2_add(struct vec2 a, struct vec2 b);
struct vec2 vec2_sub(struct vec2 a, struct vec2 b);
struct vec2 vec2_mul(struct vec2 a, struct vec2 b);
struct vec2 vec2_div(struct vec2 a, struct vec2 b);
struct vec2 vec2_scale(struct vec2 a, float b);
float vec2_dot(struct vec2 a, struct vec2 b);
float vec2_length(struct vec2 a);
struct vec2 vec2_normalize(struct vec2 a);

#endif /* !VECTORS_H */

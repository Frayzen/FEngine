#include "vector.h"
#include <math.h>

//vec4 operations

struct vec4 vec4_add(struct vec4 a, struct vec4 b){
    struct vec4 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    return result;
}

struct vec4 vec4_sub(struct vec4 a, struct vec4 b){
    struct vec4 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
    return result;
}

struct vec4 vec4_mul(struct vec4 a, struct vec4 b){
    struct vec4 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;
    return result;
}

struct vec4 vec4_div(struct vec4 a, struct vec4 b){
    struct vec4 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    result.w = a.w / b.w;
    return result;
}

struct vec4 vec4_scale(struct vec4 a, float b){
    struct vec4 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    result.w = a.w * b;
    return result;
}

float vec4_dot(struct vec4 a, struct vec4 b){
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float vec4_length(struct vec4 a){
    return sqrtf(vec4_dot(a, a));
}

struct vec4 vec4_normalize(struct vec4 a){
    float length = vec4_length(a);
    struct vec4 result;
    result.x = a.x / length;
    result.y = a.y / length;
    result.z = a.z / length;
    result.w = a.w / length;
    return result;
}

//vec3 operations

struct vec3 vec3_add(struct vec3 a, struct vec3 b){
    struct vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct vec3 vec3_sub(struct vec3 a, struct vec3 b){
    struct vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

struct vec3 vec3_mul(struct vec3 a, struct vec3 b){
    struct vec3 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
}

struct vec3 vec3_div(struct vec3 a, struct vec3 b){
    struct vec3 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    return result;
}

struct vec3 vec3_scale(struct vec3 a, float b){
    struct vec3 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    return result;
}

float vec3_dot(struct vec3 a, struct vec3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct vec3 vec3_cross(struct vec3 a, struct vec3 b){
    struct vec3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

float vec3_length(struct vec3 a){
    return sqrtf(vec3_dot(a, a));
}

struct vec3 vec3_normalize(struct vec3 a){
    float length = vec3_length(a);
    struct vec3 result;
    result.x = a.x / length;
    result.y = a.y / length;
    result.z = a.z / length;
    return result;
}

//vec2 operations

struct vec2 vec2_add(struct vec2 a, struct vec2 b){
    struct vec2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

struct vec2 vec2_sub(struct vec2 a, struct vec2 b){
    struct vec2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

struct vec2 vec2_mul(struct vec2 a, struct vec2 b){
    struct vec2 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

struct vec2 vec2_div(struct vec2 a, struct vec2 b){
    struct vec2 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return result;
}

struct vec2 vec2_scale(struct vec2 a, float b){
    struct vec2 result;
    result.x = a.x * b;
    result.y = a.y * b;
    return result;
}

float vec2_dot(struct vec2 a, struct vec2 b){
    return a.x * b.x + a.y * b.y;
}

float vec2_length(struct vec2 a){
    return sqrtf(vec2_dot(a, a));
}

struct vec2 vec2_normalize(struct vec2 a){
    float length = vec2_length(a);
    struct vec2 result;
    result.x = a.x / length;
    result.y = a.y / length;
    return result;
}

//mat4 operations

struct mat4 mat4_identity(){
    struct mat4 result;
    result.m[0][0] = 1.0f;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;
    result.m[1][0] = 0.0f;
    result.m[1][1] = 1.0f;
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;
    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = 1.0f;
    result.m[2][3] = 0.0f;
    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;
    return result;
}

#include "vector.h"
#include <math.h>

//vec4 operations

vec4 vec4_add(vec4 a, vec4 b){
    vec4 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    return result;
}

vec4 vec4_sub(vec4 a, vec4 b){
    vec4 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
    return result;
}

vec4 vec4_mul(vec4 a, vec4 b){
    vec4 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;
    return result;
}

vec4 vec4_div(vec4 a, vec4 b){
    vec4 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    result.w = a.w / b.w;
    return result;
}

vec4 vec4_scale(vec4 a, float b){
    vec4 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    result.w = a.w * b;
    return result;
}

float vec4_dot(vec4 a, vec4 b){
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float vec4_length(vec4 a){
    return sqrtf(vec4_dot(a, a));
}

vec4 vec4_normalize(vec4 a){
    float length = vec4_length(a);
    vec4 result;
    result.x = a.x / length;
    result.y = a.y / length;
    result.z = a.z / length;
    result.w = a.w / length;
    return result;
}

//vec3 operations

vec3 vec3_add(vec3 a, vec3 b){
    vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

vec3 vec3_sub(vec3 a, vec3 b){
    vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

vec3 vec3_mul(vec3 a, vec3 b){
    vec3 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
}

vec3 vec3_div(vec3 a, vec3 b){
    vec3 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    return result;
}

vec3 vec3_scale(vec3 a, float b){
    vec3 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    return result;
}

float vec3_dot(vec3 a, vec3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 vec3_cross(vec3 a, vec3 b){
    vec3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

float vec3_length(vec3 a){
    return sqrtf(vec3_dot(a, a));
}

vec3 vec3_normalize(vec3 a){
    float length = vec3_length(a);
    vec3 result;
    result.x = a.x / length;
    result.y = a.y / length;
    result.z = a.z / length;
    return result;
}

//vec2 operations

vec2 vec2_add(vec2 a, vec2 b){
    vec2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

vec2 vec2_sub(vec2 a, vec2 b){
    vec2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

vec2 vec2_mul(vec2 a, vec2 b){
    vec2 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

vec2 vec2_div(vec2 a, vec2 b){
    vec2 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return result;
}

vec2 vec2_scale(vec2 a, float b){
    vec2 result;
    result.x = a.x * b;
    result.y = a.y * b;
    return result;
}

float vec2_dot(vec2 a, vec2 b){
    return a.x * b.x + a.y * b.y;
}

float vec2_length(vec2 a){
    return sqrtf(vec2_dot(a, a));
}

vec2 vec2_normalize(vec2 a){
    float length = vec2_length(a);
    vec2 result;
    result.x = a.x / length;
    result.y = a.y / length;
    return result;
}

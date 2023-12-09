#include "vector.h"

#include <math.h>

// vec4 operations

vec4 vec4Add(vec4 a, vec4 b)
{
    vec4 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    return result;
}

vec4 vec4Sub(vec4 a, vec4 b)
{
    vec4 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
    return result;
}

vec4 vec4Mul(vec4 a, vec4 b)
{
    vec4 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;
    return result;
}

vec4 vec4Div(vec4 a, vec4 b)
{
    vec4 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    result.w = a.w / b.w;
    return result;
}

vec4 vec4Scale(vec4 a, float b)
{
    vec4 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    result.w = a.w * b;
    return result;
}

float vec4Dot(vec4 a, vec4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float vec4Length(vec4 a)
{
    return sqrtf(vec4Dot(a, a));
}

vec4 vec4Normalize(vec4 a)
{
    float length = vec4Length(a);
    vec4 result;
    result.x = a.x / length;
    result.y = a.y / length;
    result.z = a.z / length;
    result.w = a.w / length;
    return result;
}

// vec3 operations

vec3 vec3Add(vec3 a, vec3 b)
{
    vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

vec3 vec3Sub(vec3 a, vec3 b)
{
    vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

vec3 vec3Mul(vec3 a, vec3 b)
{
    vec3 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
}

vec3 vec3Div(vec3 a, vec3 b)
{
    vec3 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    return result;
}

vec3 vec3Scale(vec3 a, float b)
{
    vec3 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    return result;
}

float vec3Dot(vec3 a, vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 vec3Cross(vec3 a, vec3 b)
{
    vec3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

float vec3Length(vec3 a)
{
    return sqrtf(vec3Dot(a, a));
}

vec3 vec3Normalize(vec3 a)
{
    float length = vec3Length(a);
    vec3 result;
    result.x = a.x / length;
    result.y = a.y / length;
    result.z = a.z / length;
    return result;
}

// vec2 operations

vec2 vec2Add(vec2 a, vec2 b)
{
    vec2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

vec2 vec2Sub(vec2 a, vec2 b)
{
    vec2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

vec2 vec2Mul(vec2 a, vec2 b)
{
    vec2 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

vec2 vec2Div(vec2 a, vec2 b)
{
    vec2 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return result;
}

vec2 vec2Scale(vec2 a, float b)
{
    vec2 result;
    result.x = a.x * b;
    result.y = a.y * b;
    return result;
}

float vec2Dot(vec2 a, vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

float vec2Length(vec2 a)
{
    return sqrtf(vec2Dot(a, a));
}

vec2 vec2Normalize(vec2 a)
{
    float length = vec2Length(a);
    vec2 result;
    result.x = a.x / length;
    result.y = a.y / length;
    return result;
}

// transform operations

transform transformIdentity(void)
{
    transform result;
    result.pos = VEC3(0.0f, 0.0f, 0.0f);
    result.scale = VEC3(1.0f, 1.0f, 1.0f);
    result.rot = VEC3(0.0f, 0.0f, 0.0f);
    return result;
}

transform transformCreate(vec3 pos, vec3 scale, vec3 rot)
{
    transform result;
    result.pos = pos;
    result.scale = scale;
    result.rot = rot;
    return result;
}

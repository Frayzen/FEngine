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

typedef struct transform{
    vec3 pos;
    vec3 scale;
    vec3 rot;
} transform;

#define VEC4(X, Y, Z, W) ((vec4) {X,Y,Z,W})
vec4 vec4Add(vec4 a, vec4 b);
vec4 vec4Sub(vec4 a, vec4 b);
vec4 vec4Mul(vec4 a, vec4 b);
vec4 vec4Div(vec4 a, vec4 b);
vec4 vec4Scale(vec4 a, float b);
vec4 vec4Cross(vec4 a, vec4 b);
float vec4Dot(vec4 a, vec4 b);
float vec4Length(vec4 a);
vec4 vec4Normalize(vec4 a);

#define VEC3(X, Y, Z) ((vec3) {X,Y,Z})
vec3 vec3Create(float x, float y, float z);
vec3 vec3Add(vec3 a, vec3 b);
vec3 vec3Sub(vec3 a, vec3 b);
vec3 vec3Mul(vec3 a, vec3 b);
vec3 vec3Div(vec3 a, vec3 b);
vec3 vec3Scale(vec3 a, float b);
vec3 vec3Cross(vec3 a, vec3 b);
float vec3Dot(vec3 a, vec3 b);
float vec3Length(vec3 a);
vec3 vec3Normalize(vec3 a);

#define VEC2(X, Y) ((vec2) {X,Y})
vec3 vec3Create(float x, float y, float z);
vec2 vec2Create(float x, float y);
vec2 vec2Add(vec2 a, vec2 b);
vec2 vec2Sub(vec2 a, vec2 b);
vec2 vec2Mul(vec2 a, vec2 b);
vec2 vec2Div(vec2 a, vec2 b);
vec2 vec2Scale(vec2 a, float b);
float vec2Dot(vec2 a, vec2 b);
float vec2Length(vec2 a);
vec2 vec2Normalize(vec2 a);

transform transformIdentity(void);
transform transformCreate(vec3 pos, vec3 scale, vec3 rot);

#endif /* !VECTORS_H */

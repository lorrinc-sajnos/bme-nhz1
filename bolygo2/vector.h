#ifndef VECTOR_INC
#define VECTOR_INC
#include "vector.h"
#include "typedefs.h"

//Azt jelöli, hogy az adott vektor normálvektor, azaz hossza 1
//typedef Vec3 Vec3N

typedef struct Vec3{
    float x;
    float y;
    float z;
} Vec3;

typedef struct Pol3{
    float a; //a e [0;2PI[
    float b; //b e [-PI;PI]
    float r;
} Pol3;

const Vec3 vecNull;
const Vec3 vecOne;
const Vec3 vecMinusOne;
const Vec3 up;

Vec3 newVec(float X, float Y, float Z);
Pol3 newPol(float alpha, float beta, float radius);

Vec3 sum(Vec3 A, Vec3 B);
void sumP(Vec3 *P, Vec3 B);
Vec3 sum3(Vec3 A, Vec3 B, Vec3 C);
Vec3 sum4(Vec3 A, Vec3 B, Vec3 C, Vec3 D);

Vec3 mult(Vec3 A, float skl);
void multP(Vec3 *A, float skl);
Vec3 mn(Vec3 A);

Vec3 sub(Vec3 A, Vec3 B);

Vec3 divd(Vec3 A, float dv);


float length(Vec3 A);

Vec3 normalize(Vec3 A);

Vec3 cross(Vec3 A, Vec3 B);
float scalar(Vec3 A, Vec3 B);

float getAngle(Vec3 A, Vec3 B);

Pol3 posToPolar(Vec3 pos);
Vec3 polarToPos(Pol3 polar);
Vec3 polarToPos2(float alpha, float beta, float radius);

Pol3 coordsToPol(float x, float y, float radius);
//Vec2 polarToCoord(Vec3 polar);

float dist(Vec3 A, Vec3 B);
float distSqd(Vec3 A, Vec3 B);

Vec3 randomNormal(int32 ind, int32 seed);
Vec3 randomNormal_2D(int32 ind, int32 seed);

#endif

#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "hashfunc.h"
#include "typedefs.h"
#include "misc.h"


const Vec3 vecNull = {.x=0,.y=0,.z=0};
const Vec3 vecOne = {.x=1,.y=1,.z=1};
const Vec3 vecMinusOne = {.x=-1,.y=-1,.z=-1};

const Vec3 up = {.x=0,.y=0,.z=1};


Vec3 newVec(float X, float Y, float Z){
    Vec3 a = {.x=X,.y=Y,.z=Z};
    return a;
}

Pol3 newPol(float alpha, float beta, float radius){
    Pol3 p = {.a=alpha,.b=beta,.r=radius};
    return p;
}

Vec3 sum(Vec3 A, Vec3 B){
    return newVec(A.x+B.x,A.y+B.y,A.z+B.z);
}

void sumP(Vec3 *P, Vec3 B){
    P->x+=B.x;
    P->y+=B.y;
    P->z+=B.z;
}
Vec3 sum3(Vec3 A, Vec3 B, Vec3 C){ return sum(sum(A,B),C); }

Vec3 sum4(Vec3 A, Vec3 B, Vec3 C, Vec3 D){ return sum(sum3(A,B,C),D); }


Vec3 mult(Vec3 A, float skl){
    return newVec(A.x * skl,A.y* skl,A.z * skl);
}
void multP(Vec3 *A, float skl){
    A->x*=skl;
    A->y*=skl;
    A->z*=skl;
}

Vec3 mn(Vec3 A) {return mult(A,-1);}
Vec3 sub(Vec3 A, Vec3 B){
    return sum(A,mult(B,-1));
}

Vec3 divd(Vec3 A, float dv){
    return mult(A,1/dv);
}

float length(Vec3 A){
    return sqrtf(A.x*A.x+A.y*A.y+A.z*A.z);
}

Vec3 normalize(Vec3 A){
    return divd(A,length(A));
}

Vec3 cross(Vec3 A, Vec3 B){
    return newVec(
        A.y*B.z - A.z*B.y,
        A.z*B.x - A.x*B.z,
        A.x*B.y - A.y*B.x
        );
}

float scalar(Vec3 A, Vec3 B){
    return A.x*B.x+A.y*B.y+A.z*B.z;
}

float getAngle(Vec3 A, Vec3 B){
    float scl = scalar(A,B);
    float l_A = length(A), l_B = length(B);
    return acosf(scl/l_A/l_B);
}

float getAlpha(float x, float y){
    float alpha = x!=0.0 ? atan(y/x) : M_PI*0.5;
    alpha = alpha>0.0? alpha: M_PI+alpha;
    return y>0.0?alpha:M_PI+alpha;
}

//Átalakítja a térbeli vektort poláris koordinátává:
Pol3 posToPolar(Vec3 pos){
    float alpha = getAlpha(pos.x,pos.y); //XY sík szög
    float beta = pos.z>0.0 ? M_PI*0.5 : M_PI*-0.5; //y szög
    if(pos.x!=0.0 || pos.z!=0.0){
        float a = sqrtf(pos.x*pos.x+pos.y*pos.y);
        beta = atan(pos.z/a);
    }
    float r = length(pos);
    return  newPol(alpha,beta,r);
}

//Átalakítja a poláris koordinátát térbeli vektorrá
Vec3 polarToPos(Pol3 polar){ return polarToPos2(polar.a,polar.b,polar.r); }

Vec3 polarToPos2(float alpha, float beta, float radius){
    float z = sin(beta)*radius;
    float a = cos(beta)*radius;
    float x = cos(alpha)*a;
    float y = sin(alpha)*a;
    return newVec(x,y,z);

}

//x e [0;1], y e [0;1]
Pol3 coordsToPol(float x, float y, float radius){
    float alpha = x*2*M_PI;
    float beta = (y-0.5)*M_PI;
    return newPol(alpha,beta,radius);
}

//Két vektor négyzetes távolsága
float distSqd(Vec3 A, Vec3 B){
    Vec3 C = sub(B,A);
    return C.x*C.x+C.y*C.y+C.z*C.z;
}

float dist(Vec3 A, Vec3 B) { return sqrtf(distSqd(A,B)); }

Vec3 randomNormal(int32 ind, int32 seed){
    //Nagyjából véletlenszerű
    float alpha = sc01(hash_f(ind,0,0,seed))*M_PI*2;
    float beta = hash_f(ind,0,ind,seed-1)*M_PI;
    //float beta2 = 4*beta*beta*beta-6*beta*beta+3*beta;

    return polarToPos2(alpha,beta,1);
}


Vec3 randomNormal_2D(int32 ind, int32 seed){
    float alpha = hash_f(ind,ind,0,seed);
    return newVec(cosf(alpha),sinf(alpha),0);
}

#include <string.h>

#include "debugmalloc.h"
#include "misc.h"
#include "typedefs.h"

float linint(float x, float y, float t) {
	return x + (y - x) * t;
}
float easef(float t) {
	return t * t * (3 - 2 * t);
}

float ushortToPrc(ushort val){
    return val/USHORT_MAX_F;
}
ushort prcToUshort(float prc){
    return (ushort)(prc * USHORT_MAX_F);
}

float sc01(float x) {
	return x*0.5+0.5;
}
float sc11(float x) {
	return x*2-1;
}
float scale(float x, float min, float max) {
	return (x-min)/(max-min);
}


float scaleInf(float x) {
	return scaleInf2(x,0.25);
}
float scaleInf2(float x, float ampl) {
	return x /(x+ampl);
}

//Forrás: (double helyett float)
//https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value
float clamp(float d, float min, float max) {
	const float t = d < min ? min : d;
	return t > max ? max : t;
}

float cubicInt(float x) {
	float t = x-1;
	return t*t*t+1;
}

float bell(float x) {
	return 4*x-4*x*x;
}

int posmod(int x, int mod){
    int val = x%mod;
    return val>=0?val:val+mod;
}


int64 encStr(char *str){
    int64 code = 0;
    for(int i=0;i<12 && str[i] != '\0';i++){
        code<<=5;
        if((str[i]<'a' || str[i]>'z') && str[i]!='_')
            return -1;
        code += str[i] == '_'? 31 : str[i]-'a'+1;
    }
    return code;
}

void decStr(char *str, int64 code){
    int letter = 0;
    int cntr = 0;
    while(code!=0){
        letter = code%32;
        str[cntr] = letter == 31? '_' : letter-1+'a';
        code>>=5;
        cntr++;
    }
    str[cntr]='\0';
    strrev(str);
}

void strcut(char *origin, char *first, char *rest){
    bool spaceWasHit = false;
    char *dest = first;
    for(int i=0;origin[i]!='\0';i++){
        if(!spaceWasHit && origin[i]==' '){
            spaceWasHit=true;
            *dest='\0';
            dest = rest;
        }
        else
            *dest++=origin[i];
    }
    *dest='\0';

}

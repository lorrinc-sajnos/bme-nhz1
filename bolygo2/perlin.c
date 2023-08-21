#define X_PRIME 353594023
#define Y_PRIME 1465077127
#define Z_PRIME 2082801727
#include "hashfunc.h"
#include "misc.h"

//Perlin algoritumsa alapján
//https://en.wikipedia.org/wiki/Perlin_noise
//https://adrianb.io/2014/08/09/perlinnoise.html

int mod(int a, int m){
    int x = a % m;
    return x > 0 ? x : x + m;
}

float dotGrad(int hashX, int hashY, int hashZ,float x, float y, float z, int32 seed);

float perlin(float x, float y, float z, int seed){
    int xi = x > 0 ? (int)x : (int)x - 1;
    int yi = y > 0 ? (int)y : (int)y - 1;
    int zi = z > 0 ? (int)z : (int)z - 1;

    float xr = x - xi;
    float yr = y - yi;
    float zr = z - zi;

    //int xi1 = xi + 1, yi1 = yi + 1, zi1 = zi + 1;
    float xr_1 = xr - 1, yr_1 = yr - 1, zr_1 = zr - 1;

    //kiszámoljuk az összes vektor skaláris szorzatát
    float d000 = dotGrad(xi, yi, zi, xr, yr, zr, seed);
    float d100 = dotGrad(xi+1, yi, zi, xr_1, yr, zr, seed);
    float d001 = dotGrad(xi, yi, zi+1, xr, yr, zr_1, seed);
    float d101 = dotGrad(xi+1, yi, zi+1, xr_1, yr, zr_1, seed);

    float d010 = dotGrad(xi, yi+1, zi, xr, yr_1, zr, seed);
    float d110 = dotGrad(xi+1, yi+1, zi, xr_1, yr_1, zr, seed);
    float d011 = dotGrad(xi, yi+1, zi+1, xr, yr_1, zr_1, seed);
    float d111 = dotGrad(xi+1, yi+1, zi+1, xr_1, yr_1, zr_1, seed);

    float A, B, C, D, P, Q;

    /*
    float easeXT = xr * xr * (3 - 2 * xr);
    float easeYT = yr * yr * (3 - 2 * yr);
    float easeZT = zr * zr * (3 - 2 * zr);
    */

    float easeXT = easef(xr);
    float easeYT = easef(yr);
    float easeZT = easef(zr);


    A = linint(d000, d100, easeXT);
    B = linint(d010, d110, easeXT);
    C = linint(d001, d101, easeXT);
    D = linint(d011, d111, easeXT);

    P = linint(A, B, easeYT);
    Q = linint(C, D, easeYT);

    //return linint(P, Q, zr);
    return linint(P, Q, easeZT);
}

float dotGrad(int indX, int indY, int indZ, float x, float y, float z, int32 seed){

    int32 hash = hash_i(indX,indY,indZ,seed);
    hash = (hash>>4)%18;
    //Az a lényeg hogy iytt a vektorok
    //korrdinátái csak -1,0 és 1, ezért nem
    //is írjuk oda, hanem elõjellel helyettesítjük,
    //mivel összeadni gyorsabb mint szorozni
    hash = hash < 0 ? -hash : hash;
    switch (hash)    {
        default:
    case 0:
        return -x - y;

    case 1:
        return -x - z;

    case 2:
        return -x;

    case 3:
        return -x + z;

    case 4:
        return -x + y;

    case 5:
        return -y - z;

    case 6:
        return -y;

    case 7:
        return -y + z;

    case 8:
        return -z;

    case 9:
        return z;

    case 10:
        return y - z;

    case 11:
        return y;

    case 12:
        return y + z;

    case 13:
        return x - y;

    case 14:
        return x - z;

    case 15:
        return x;

    case 16:
        return x + z;

    case 17:
        return x + y;
    }

}

float layeredPerlin(float x, float y, float z, int32 seed, float layerWeights[], float layerScales[], int layerCount){
    float noise = 0;
	for(int i=0; i<layerCount; i++) {
		float weight = layerWeights[i];
		float scale = layerScales[i];
		float nx = x/scale, ny = y/scale, nz = z/scale;
		noise+=sc01(perlin(nx,ny,nz,seed+2*i))*weight;
	}
	return noise;
}

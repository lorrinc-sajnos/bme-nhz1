#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "debugmalloc.h"

#include "perlin.h"
#include "hashfunc.h"
#include "vector.h"
#include "misc.h"

#include "heightgeneration.h"
#include "generationsettings.h"
#include "planetdata.h"
#include "typedefs.h"

Vec3 applyOffset(Vec3 pos, Vec3 offset2D, float scale);
Vec3 perlinOffset(Vec3 vec, int32 seed);

//Forrás:
//https://timcoster.com/2020/02/13/raymarching-shader-pt3-smooth-blending-operators/
float blendDist(float distA, float distB, float k ) {
  float h = clamp(0.5 + 0.5*(distA-distB)/k, 0., 1.);
  return linint(distA, distB, h) - k*h*(1.-h);
}

void generateHeightData(PlanetData* planetData, GenSettings* genSettings){
    genSettings-> genInProgress=true;

    const int32 seed = genSettings->seed;
    int width = planetData->dataW;
    int height = planetData->dataH;
    ushort **heightData = planetData->heightData;


    //Kontinens pontajinak meghatározása
    int continentCount = genSettings->continentCount;

    int continentSize[continentCount];

    //minimum kontinens darabok száma, és hogy mennyi a variáció
    int min = genSettings->minContinentNodes;
    int dif = genSettings->maxContinentNodes-min+1;

    float nDistMin = genSettings->minNodeDist;
    float nDistDif = genSettings->maxNodeDist-nDistMin;

    //Kontinensek tömbje, kontinens: pont-tömb
    Vec3 **continentNodes = malloc(continentCount*sizeof(Vec3*));
    for(int i=0;i<continentCount;i++){
        //Véletlenszerû kontinensméret
        continentSize[i] = (int)(min+dif*sc01(hash_f(0,i,0,seed)));
        continentNodes[i] = (Vec3*)malloc(continentSize[i]*sizeof(Vec3));

        //Kontinens fõpontja
        continentNodes[i][0] = randomNormal(i,seed);
        for(int j=1;j<continentSize[i];j++){
            Vec3 offset2D = randomNormal_2D(j,seed+i);
            float nodeD = nDistMin+nDistDif*sc01(hash_f(j,i,j+i,seed));
            continentNodes[i][j] = applyOffset(continentNodes[i][0],offset2D,nodeD);
        }
    }


    //*
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){

            Vec3 pos = planetData->sphereMap[y][x];
            Vec3 checkPos = pos;

            for(int j=0;j<genSettings->edgeLayerCount;j++){
                float weight = *(genSettings->edgeWgh+j);
                sumP(&checkPos,mult(perlinOffset(checkPos,seed+2*j),weight));

            }

            //Spórolás végett eltároljuk a min. távot is
            //Kezdõértéknek illegális távod adunk meg
            //(mivel az összes vektor egységvektor, ezért 999 nem lehet a távjuk):

            int min1_i = -1;
            float min1_d= 999;
            int min2_i = -1;
            float min2_d= 999;

            //Távolságkeresés
            for(int i=0;i<continentCount;i++){
                for(int j=0;j<continentSize[i];j++){
                    float temp_d = distSqd(checkPos, continentNodes[i][j]);
                    if(temp_d<min2_d){
                        if(temp_d<min1_d){
                            min2_i = min1_i;
                            min2_d = min1_d;

                            min1_d = temp_d;
                            min1_i = i;
                        }
                        else{
                            min2_i = i;
                            min2_d = temp_d;
                        }
                    }
                }
            }
            float blendedDist = blendDist(min1_d,min2_d,0.1);
            //float blendedDist = blendDist(scaleInf(min1_d),scaleInf(min2_d),0.1);
            float f = 1-scaleInf(blendedDist);

            float noise = layeredPerlin(pos.x,pos.y,pos.z,seed,genSettings->terrWgh,genSettings->terrScl,genSettings->terrLayerCount);
            /*
            float noise = 0;
            for(int j=0;j<genSettings->terrLayerCount;j++){
                float scale = genSettings->terrScl[j];
                float weight = genSettings->terrWgh[j];
                float nx = pos.x/scale, ny = pos.y/scale, nz = pos.z/scale;
                noise+=sc01(perlin(nx,ny,nz,seed+2*j))*weight;

            }*/

            f=f*(0.3+0.7*noise);
            f=easef(f);



            heightData[y][x] = prcToUshort(f);
        }
    }

    //Kontinens pozíciók felszabadítása
    for(int i=0;i<continentCount;i++)
        free(continentNodes[i]);
    free(continentNodes);


    genSettings->genInProgress=false;
    return;
}


Vec3 perlinOffset(Vec3 vec, int32 seed){
    float x = perlin(vec.x,vec.y,vec.z,seed+13);
    float y = perlin(vec.x,vec.y,vec.z,seed+84);
    float z = perlin(vec.x,vec.y,vec.z,seed+72);
    return newVec(x,y,z);
}

Vec3 applyOffset(Vec3 pos, Vec3 offset2D, float scale){
    Pol3 posP = posToPolar(pos);

    Vec3 right = polarToPos2(posP.a+M_PI,posP.b/2.0,1);
    Vec3 up = polarToPos2(posP.a,posP.b+M_PI/2.0,1);

    Vec3 offsetVec = normalize(sum(
                                   mult(right,offset2D.x),
                                   mult(up,offset2D.y)
                                   ));

    return sum(pos,mult(offsetVec,scale));
}

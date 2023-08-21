#define X_PRIME 558871463
#define Y_PRIME 1713964481
#define Z_PRIME 1574095141

#include "typedefs.h"

int32 hash_i(int32 x, int32 y, int32 z, int32 seed){
    int32 hash = seed;
    hash ^= X_PRIME * x;
    hash ^= Y_PRIME * y;
    hash ^= Z_PRIME * z;

    return hash*hash*hash;
}
float hash_f(int32 x, int32 y, int32 z, int32 seed){
    return hash_i(x,y,z,seed)/INT32_MAX_F;
}


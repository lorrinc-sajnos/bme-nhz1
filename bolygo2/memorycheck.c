//Itt az�rt nincs import�lva a debugmalloc, mert adott b�jtsz�m f�l�tt nem enged lefoglalni
#include <stdlib.h>

int memoryCheck(long megabytes){
    void *temp = malloc(megabytes*1000000);
    if(temp==NULL)
        return -1;
    free(temp);
    return 1;
}

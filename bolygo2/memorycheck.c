//Itt azért nincs importálva a debugmalloc, mert adott bájtszám fölött nem enged lefoglalni
#include <stdlib.h>

int memoryCheck(long megabytes){
    void *temp = malloc(megabytes*1000000);
    if(temp==NULL)
        return -1;
    free(temp);
    return 1;
}

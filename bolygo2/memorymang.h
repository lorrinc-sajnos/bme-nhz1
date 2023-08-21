#ifndef MEMORYMANG_INC
#define MEMORYMANG_INC
void** malloc2D(int width, int height, long unsigned int size);

void free2D(void** pntr2d, int height);

#endif

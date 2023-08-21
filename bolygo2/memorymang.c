#include "debugmalloc.h"

void** malloc2D(int width, int height, long unsigned int size) {

	void** array2d = (void**)malloc(height*sizeof(void*));

	for(int i=0; i<height; i++) {
		array2d[i] = malloc(width*size);
	}
	return array2d;
}

void free2D(void** pntr2d, int height) {
	for(int i=0; i<height; i++) {
		free(pntr2d[i]);
	}
	free(pntr2d);
}

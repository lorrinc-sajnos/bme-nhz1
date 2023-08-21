#ifndef COLORLIST_INC
#define COLORLIST_INC
#include "rgb.h"

#define COLORLIST_SIZE 5
typedef struct ColorList{
    float height;
    RGB color;
    struct ColorList* nextColor;
} ColorList;

ColorList* addToEnd(ColorList *lnkList, float value, RGB color);
ColorList* addToEnd2(ColorList *lnkList, ColorList element);
ColorList* addAt(ColorList *listElement, float value, RGB color);

int getSize(ColorList* lnkList);

ColorList* getElement(ColorList *lnkList, int index);
int getIndex(ColorList *lnkList, RGB color);


ColorList* getElementByColor(ColorList *lnkList, RGB color);
ColorList *removeElement(ColorList *lnkList, int index);

void freeList(ColorList *lnkList);

#endif

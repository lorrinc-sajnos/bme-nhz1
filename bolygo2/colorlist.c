#include <math.h>
#include <stdbool.h>
#include "colorlist.h"
#include "rgb.h"
#include "debugmalloc.h"
#include "rgb.h"


ColorList* addToEnd(ColorList *lnkList, float value, RGB color){
    ColorList *p = lnkList;
    ColorList *elementPnt = (ColorList*)malloc(sizeof(ColorList));

    elementPnt->nextColor=NULL;
    elementPnt->height=value;
    elementPnt->color=color;

    if(p == NULL)
        return elementPnt;

    while(p->nextColor!=NULL)
        p=p->nextColor;
    p->nextColor=elementPnt;
    return lnkList;
}

ColorList* addToEnd2(ColorList *lnkList, ColorList element){ return addToEnd(lnkList,element.height, element.color);}

int getSize(ColorList* lnkList){
    ColorList *p = lnkList;
    int counter = 0;
    while(p!=NULL){
        p=p->nextColor;
        counter++;
    }
    return counter;
}

ColorList* addAt(ColorList *listElement, float value, RGB color){
    if(listElement==NULL)
        return NULL;
    ColorList *newElement = (ColorList*)malloc(sizeof(ColorList));
    newElement->height=value;
    newElement->color=color;

    newElement->nextColor=listElement->nextColor;
    listElement->nextColor=newElement;

    return listElement;
}

ColorList* getElement(ColorList *lnkList, int index){
    ColorList *p = lnkList;
    for(int i=0;i<index;i++){
        if(p==NULL) return NULL;
        p=p->nextColor;
    }
    return p;
}

int getIndex(ColorList *lnkList, RGB color){
    ColorList *p = lnkList;
    int cnt = 0;
    while(p!=NULL){
        if(equals(p->color,color))
            return cnt;
        p=p->nextColor;
        cnt++;
    }
    return -1;
}


ColorList* getElementByColor(ColorList *lnkList, RGB color){
    ColorList *p = lnkList;
    while(p!=NULL){
        if(equals(p->color,color))
            return p;
        p=p->nextColor;
    }
    return NULL;
}

ColorList *removeElement(ColorList *lnkList, int index){
    if(lnkList==NULL) return NULL;
    if(index == 0){
        ColorList *temp=lnkList->nextColor;
        free(lnkList);//
        return temp;
    }
    ColorList *prevElement = getElement(lnkList,index-1);
    ColorList *deleteElement = prevElement->nextColor;
    if(deleteElement==NULL)
        return lnkList;

    prevElement->nextColor=deleteElement->nextColor;
    free(deleteElement);

    return lnkList;
}


void freeList(ColorList *lnkList){
    ColorList *p = lnkList;
    ColorList *temp;

    while (p!=NULL){
        temp = p->nextColor;
        free(p);
        p = temp;
    }

}


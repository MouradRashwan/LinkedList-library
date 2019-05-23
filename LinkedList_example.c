#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "LinkedList.h"

#define LINKED_LIST_LENGTH                5
#define LINKED_LIST_STATIC_ALLOCATION     0

typedef struct Object
{
    int32_t i32Val1;
    int32_t i32Val2;
} Object_t;

int main(void)
{
    printf("LinkedList Starting ...\n");

    int32_t ii;
    LinkedList_t tLinkedList;
    Object_t tObject, tAfterThisObj, tBeforeThisObj;

#if(LINKED_LIST_STATIC_ALLOCATION == 0)
    if(LinkedList_allocateDynamic(&tLinkedList, LINKED_LIST_LENGTH, sizeof(Object_t)) == false)
    {
        printf("<<ERROR>> Heap Memory is Full");
    }
#else
    static ElementHeader_t atElemHeader[LINKED_LIST_LENGTH];
    static Object_t atObject[LINKED_LIST_LENGTH];
    LinkedList_allocateStatic(&tLinkedList, atElemHeader, atObject, LINKED_LIST_LENGTH, sizeof(Object_t));
#endif

    for(ii=0; ii<LINKED_LIST_LENGTH; ii++)
    {
        tObject.i32Val1 = ii + 1;
        tObject.i32Val2 = (ii + 1) * 10;
        LinkedList_insertEnd(&tLinkedList, &tObject);
    }

    LinkedList_getHead(&tLinkedList, &tObject);
    printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    while(LinkedList_getNext(&tLinkedList, &tObject))
    {
        printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    }
    printf("\n\n");

    tObject.i32Val1 = 1;
    tObject.i32Val2 = 10;
    LinkedList_remove(&tLinkedList, &tObject);

    LinkedList_getHead(&tLinkedList, &tObject);
    printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    while(LinkedList_getNext(&tLinkedList, &tObject))
    {
        printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    }
    printf("\n\n");

    tObject.i32Val1 = 5;
    tObject.i32Val2 = 50;
    LinkedList_remove(&tLinkedList, &tObject);

    LinkedList_getHead(&tLinkedList, &tObject);
    printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    while(LinkedList_getNext(&tLinkedList, &tObject))
    {
        printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    }
    printf("\n\n");

    tObject.i32Val1 = 9;
    tObject.i32Val2 = 90;
    tAfterThisObj.i32Val1 = 2;
    tAfterThisObj.i32Val2 = 20;
    LinkedList_insertAfter(&tLinkedList, &tObject, &tAfterThisObj);

    LinkedList_getHead(&tLinkedList, &tObject);
    printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    while(LinkedList_getNext(&tLinkedList, &tObject))
    {
        printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    }
    printf("\n\n");

    tObject.i32Val1 = 9;
    tObject.i32Val2 = 90;
    tBeforeThisObj.i32Val1 = 4;
    tBeforeThisObj.i32Val2 = 40;
    LinkedList_insertBefore(&tLinkedList, &tObject, &tBeforeThisObj);

    LinkedList_getHead(&tLinkedList, &tObject);
    printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    while(LinkedList_getNext(&tLinkedList, &tObject))
    {
        printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    }
    printf("\n\n");

    LinkedList_getEnd(&tLinkedList, &tObject);
    printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    while(LinkedList_getNext(&tLinkedList, &tObject))
    {
        printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    }
    printf("\n\n");

#if(LINKED_LIST_STATIC_ALLOCATION == 0)
    LinkedList_deallocateDynamic(&tLinkedList);
#endif

    return 0;
}

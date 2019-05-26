#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "LinkedList.h"

#define LINKED_LIST_LENGTH              3
#define NEW_LINKED_LIST_LENGTH          7
#define LINKED_LIST_STATIC_ALLOCATION   0

typedef struct Object
{
    int32_t i32Val1;
    int32_t i32Val2;
} Object_t;

int main(void)
{
    printf("LinkedList Starting ...\n\n");

    int32_t ii;
    LinkedList_t tLinkedList;
    Object_t tObject, tAfterThisObj, tBeforeThisObj;

#if(LINKED_LIST_STATIC_ALLOCATION == 0)
    if(LinkedList_allocateDynamic(&tLinkedList, LINKED_LIST_LENGTH, sizeof(Object_t)) == false)
    {
        printf("<<ERROR>> Dynamic Allocation Failed.\n\n");
        return 0;
    }
#else
    static ElementHeader_t atElemHeader[LINKED_LIST_LENGTH];
    static Object_t atObject[LINKED_LIST_LENGTH];
    LinkedList_allocateStatic(&tLinkedList, atElemHeader, atObject, LINKED_LIST_LENGTH, sizeof(Object_t));
#endif

    tObject.i32Val1 = 0;
    tObject.i32Val2 = 0;
    LinkedList_add(&tLinkedList, &tObject);
    tObject.i32Val1 = 111;
    tObject.i32Val2 = 111;
    LinkedList_add(&tLinkedList, &tObject);
    tObject.i32Val1 = 0;
    tObject.i32Val2 = 0;
    LinkedList_add(&tLinkedList, &tObject);

    printf("Old Addresses: [0x%08X | 0x%08X]\n", (uint32_t)tLinkedList.pvArray, (uint32_t)tLinkedList.ptElemHeaderArray);

    if(LinkedList_reallocateDynamic(&tLinkedList, NEW_LINKED_LIST_LENGTH) == false)
    {
        printf("<<ERROR>> Dynamic Reallocation Failed.\n\n");
    }

    printf("New Addresses: [0x%08X | 0x%08X]\n\n", (uint32_t)tLinkedList.pvArray, (uint32_t)tLinkedList.ptElemHeaderArray);

    LinkedList_getHead(&tLinkedList, &tObject);
    printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    while(LinkedList_getNext(&tLinkedList, &tObject))
    {
        printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    }
    printf("\n\n");

    for(ii=0; ii<2; ii++)
    {
        tObject.i32Val1 = ii + 3;
        tObject.i32Val2 = (ii + 3) * 10;
        LinkedList_insertAtEnd(&tLinkedList, &tObject);
    }
    for(ii=0; ii<2; ii++)
    {
        tObject.i32Val1 = 2 - ii;
        tObject.i32Val2 = (2 - ii) * 10;
        LinkedList_insertAtHead(&tLinkedList, &tObject);
    }

    LinkedList_getHead(&tLinkedList, &tObject);
    printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    while(LinkedList_getNext(&tLinkedList, &tObject))
    {
        printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    }
    printf("\n\n");

    LinkedList_removeHead(&tLinkedList);
    LinkedList_removeHead(&tLinkedList);
    LinkedList_removeEnd(&tLinkedList);
    LinkedList_removeEnd(&tLinkedList);

    LinkedList_getHead(&tLinkedList, &tObject);
    printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    while(LinkedList_getNext(&tLinkedList, &tObject))
    {
        printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    }
    printf("\n\n");

    tObject.i32Val1 = 1;
    tObject.i32Val2 = 10;
    LinkedList_insertAtHead(&tLinkedList, &tObject);

    tObject.i32Val1 = 4;
    tObject.i32Val2 = 40;
    LinkedList_insertAtEnd(&tLinkedList, &tObject);

    LinkedList_getHead(&tLinkedList, &tObject);
    printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    while(LinkedList_getNext(&tLinkedList, &tObject))
    {
        printf("(%u|%u) ", tObject.i32Val1, tObject.i32Val2);
    }
    printf("\n\n");

    tObject.i32Val1 = 9;
    tObject.i32Val2 = 90;
    tAfterThisObj.i32Val1 = 4;
    tAfterThisObj.i32Val2 = 40;
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
    tBeforeThisObj.i32Val1 = 1;
    tBeforeThisObj.i32Val2 = 10;
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

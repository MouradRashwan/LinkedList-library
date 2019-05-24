#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "LinkedList.h"

#define LINKED_LIST_LENGTH                6
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

    for(ii=0; ii<LINKED_LIST_LENGTH-3; ii++)
    {
        tObject.i32Val1 = ii + 4;
        tObject.i32Val2 = (ii + 4) * 10;
        LinkedList_insertAtEnd(&tLinkedList, &tObject);
    }
    for(ii=0; ii<LINKED_LIST_LENGTH-3; ii++)
    {
        tObject.i32Val1 = 3 - ii;
        tObject.i32Val2 = (3 - ii) * 10;
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

    tObject.i32Val1 = 2;
    tObject.i32Val2 = 20;
    LinkedList_insertAtHead(&tLinkedList, &tObject);

    tObject.i32Val1 = 5;
    tObject.i32Val2 = 50;
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
    tAfterThisObj.i32Val1 = 5;
    tAfterThisObj.i32Val2 = 50;
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
    tBeforeThisObj.i32Val1 = 2;
    tBeforeThisObj.i32Val2 = 20;
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

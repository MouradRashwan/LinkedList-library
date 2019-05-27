#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

typedef struct ElementHeader
{
    bool bValid;
    int32_t i32NextIndex;
    int32_t i32PreviousIndex;
} ElementHeader_t;

typedef struct LinkedList
{
    int32_t i32Head;
    int32_t i32End;
    int32_t i32LinkedListLen;
    int32_t i32ActualLen;
    int32_t i32ElementSize;
    int32_t i32Next;
    bool bHeadingToEnd;
    void *pvArray;
    ElementHeader_t *ptElemHeaderArray;
} LinkedList_t;

void LinkedList_allocateStatic(LinkedList_t *ptLinkedList, ElementHeader_t *ptElemHeaderArray, void *pvLinkedListArray, int32_t i32LinkedListLen, int32_t i32ElementSize);

bool LinkedList_allocateDynamic(LinkedList_t *ptLinkedList, int32_t i32LinkedListLen, int32_t i32ElementSize);

bool LinkedList_reallocateDynamic(LinkedList_t *ptLinkedList, int32_t i32NewLinkedListLen);

void LinkedList_deallocateDynamic(LinkedList_t *ptLinkedList);

bool LinkedList_isEmpty(LinkedList_t *ptLinkedList);

bool LinkedList_isFull(LinkedList_t *ptLinkedList);

int32_t LinkedList_getActualLen(LinkedList_t *ptLinkedList);

int32_t LinkedList_getLinkedListLen(LinkedList_t *ptLinkedList);

bool LinkedList_getHead(LinkedList_t *ptLinkedList, void *pvElement);

bool LinkedList_getEnd(LinkedList_t *ptLinkedList, void *pvElement);

bool LinkedList_getNext(LinkedList_t *ptLinkedList, void *pvElement);

int32_t LinkedList_find(LinkedList_t *ptLinkedList, void *pvElement);

bool LinkedList_add(LinkedList_t *ptLinkedList, void *pvElement);

bool LinkedList_remove(LinkedList_t *ptLinkedList, void *pvElement);

bool LinkedList_removeHead(LinkedList_t *ptLinkedList);

bool LinkedList_removeEnd(LinkedList_t *ptLinkedList);

bool LinkedList_insertAtHead(LinkedList_t *ptLinkedList, void *pvElement);

bool LinkedList_insertAtEnd(LinkedList_t *ptLinkedList, void *pvElement);

bool LinkedList_insertAfter(LinkedList_t *ptLinkedList, void *pvElement, void *pvAfterThisElem);

bool LinkedList_insertBefore(LinkedList_t *ptLinkedList, void *pvElement, void *pvBeforeThisElem);

#endif // LINKED_LIST_H_INCLUDED

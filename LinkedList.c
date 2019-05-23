#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

static int32_t _getValidIndex(LinkedList_t *ptLinkedList)
{
    int32_t ii;

    for(ii=0; ii<ptLinkedList->i32LinkedListLen; ii++)
    {
        if(ptLinkedList->ptElemHeaderArray[ii].bValid == false)
        {
            break;
        }
    }

    if(ii == ptLinkedList->i32LinkedListLen)
    {
        ii = -1;
    }

    return ii;
}

static int32_t _getEndIndex(LinkedList_t *ptLinkedList)
{
    int32_t i32CurrElemIndex, i32LastElemIndex;

    i32LastElemIndex = -1;
    i32CurrElemIndex = ptLinkedList->i32Head;
    while(i32CurrElemIndex != -1)
    {
        i32LastElemIndex = i32CurrElemIndex;
        i32CurrElemIndex = ptLinkedList->ptElemHeaderArray[i32CurrElemIndex].i32NextIndex;
    }

    return i32LastElemIndex;
}

void LinkedList_allocateStatic(LinkedList_t *ptLinkedList, ElementHeader_t *ptElemHeaderArray, void *pvLinkedListArray, int32_t i32LinkedListLen, int32_t i32ElementSize)
{
    int32_t ii;

    ptLinkedList->i32ElementSize = i32ElementSize;
    ptLinkedList->i32Head = -1;
    ptLinkedList->i32ActualLen = 0;
    ptLinkedList->i32LinkedListLen = i32LinkedListLen;
    ptLinkedList->pvArray = pvLinkedListArray;
    ptLinkedList->ptElemHeaderArray = ptElemHeaderArray;

    for(ii=0; ii<ptLinkedList->i32LinkedListLen; ii++)
    {
        ptLinkedList->ptElemHeaderArray[ii].bValid = false;
        ptLinkedList->ptElemHeaderArray[ii].i32NextIndex = -1;
        ptLinkedList->ptElemHeaderArray[ii].i32PreviousIndex = -1;
    }
}

bool LinkedList_allocateDynamic(LinkedList_t *ptLinkedList, int32_t i32LinkedListLen, int32_t i32ElementSize)
{
    int32_t ii;

    ptLinkedList->i32ElementSize = i32ElementSize;
    ptLinkedList->i32Head = -1;
    ptLinkedList->i32ActualLen = 0;
    ptLinkedList->i32LinkedListLen = i32LinkedListLen;
    ptLinkedList->pvArray = (void *)calloc(ptLinkedList->i32LinkedListLen, ptLinkedList->i32ElementSize);
    ptLinkedList->ptElemHeaderArray = (ElementHeader_t *)calloc(ptLinkedList->i32LinkedListLen, sizeof(ElementHeader_t));

    if((ptLinkedList->pvArray == NULL) || (ptLinkedList->ptElemHeaderArray == NULL))
    {
        free(ptLinkedList->pvArray);
        free(ptLinkedList->ptElemHeaderArray);
        return false;
    }

    for(ii=0; ii<ptLinkedList->i32LinkedListLen; ii++)
    {
        ptLinkedList->ptElemHeaderArray[ii].bValid = false;
        ptLinkedList->ptElemHeaderArray[ii].i32NextIndex = -1;
        ptLinkedList->ptElemHeaderArray[ii].i32PreviousIndex = -1;
    }

    return true;
}

void LinkedList_deallocateDynamic(LinkedList_t *ptLinkedList)
{
    free(ptLinkedList->pvArray);
    free(ptLinkedList->ptElemHeaderArray);
    ptLinkedList->pvArray = NULL;
    ptLinkedList->ptElemHeaderArray = NULL;
}

bool LinkedList_isEmpty(LinkedList_t *ptLinkedList)
{
    if(ptLinkedList->i32ActualLen == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool LinkedList_isFull(LinkedList_t *ptLinkedList)
{
    if(ptLinkedList->i32ActualLen == ptLinkedList->i32LinkedListLen)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int32_t LinkedList_getActualLen(LinkedList_t *ptLinkedList)
{
    return ptLinkedList->i32ActualLen;
}

int32_t LinkedList_getLinkedListLen(LinkedList_t *ptLinkedList)
{
    return ptLinkedList->i32LinkedListLen;
}

bool LinkedList_getHead(LinkedList_t *ptLinkedList, void *pvElement)
{
    void *pvLinkedListElement;

    ptLinkedList->bHeadingToEnd = true;

    if(!LinkedList_isEmpty(ptLinkedList))
    {
        ptLinkedList->i32Next = ptLinkedList->ptElemHeaderArray[ptLinkedList->i32Head].i32NextIndex;
        {
            pvLinkedListElement = ((uint8_t *)ptLinkedList->pvArray) + (ptLinkedList->i32Head * ptLinkedList->i32ElementSize);
            memcpy(pvElement, pvLinkedListElement, ptLinkedList->i32ElementSize);
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool LinkedList_getEnd(LinkedList_t *ptLinkedList, void *pvElement)
{
    int32_t i32EndElemIndex;
    void *pvLinkedListElement;

    ptLinkedList->bHeadingToEnd = false;

    if(!LinkedList_isEmpty(ptLinkedList))
    {
        i32EndElemIndex = _getEndIndex(ptLinkedList);
        ptLinkedList->i32Next = ptLinkedList->ptElemHeaderArray[i32EndElemIndex].i32PreviousIndex;
        {
            pvLinkedListElement = ((uint8_t *)ptLinkedList->pvArray) + (i32EndElemIndex * ptLinkedList->i32ElementSize);
            memcpy(pvElement, pvLinkedListElement, ptLinkedList->i32ElementSize);
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool LinkedList_getNext(LinkedList_t *ptLinkedList, void *pvElement)
{
    void *pvLinkedListElement;

    if(ptLinkedList->i32Next != -1)
    {
        {
            pvLinkedListElement = ((uint8_t *)ptLinkedList->pvArray) + (ptLinkedList->i32Next * ptLinkedList->i32ElementSize);
            memcpy(pvElement, pvLinkedListElement, ptLinkedList->i32ElementSize);
        }
        if(ptLinkedList->bHeadingToEnd == true)
        {
            ptLinkedList->i32Next = ptLinkedList->ptElemHeaderArray[ptLinkedList->i32Next].i32NextIndex;
        }
        else
        {
            ptLinkedList->i32Next = ptLinkedList->ptElemHeaderArray[ptLinkedList->i32Next].i32PreviousIndex;
        }
        return true;
    }
    else
    {
        return false;
    }
}

int32_t LinkedList_find(LinkedList_t *ptLinkedList, void *pvElement)
{
    int32_t i32ElementIndex;
    void *pvLinkedListElement;

    i32ElementIndex = ptLinkedList->i32Head;
    while(i32ElementIndex != -1)
    {
        pvLinkedListElement = ((uint8_t *)ptLinkedList->pvArray) + (i32ElementIndex * ptLinkedList->i32ElementSize);

        if(memcmp(pvElement, pvLinkedListElement, ptLinkedList->i32ElementSize) == 0)
        {
            break;
        }

        i32ElementIndex = ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32NextIndex;
    }

    return i32ElementIndex;
}

bool LinkedList_add(LinkedList_t *ptLinkedList, void *pvElement)
{
    int32_t i32ElementIndex1, i32ElementIndex2;
    void *pvLinkedListElement;

    if(!LinkedList_isFull(ptLinkedList))
    {
        if(ptLinkedList->i32Head == -1)
        {
            ptLinkedList->i32Head = 0;
            ptLinkedList->ptElemHeaderArray[0].i32NextIndex = -1;
            ptLinkedList->ptElemHeaderArray[0].i32PreviousIndex = -1;
            ptLinkedList->ptElemHeaderArray[0].bValid = true;
            {
                pvLinkedListElement = ((uint8_t *)ptLinkedList->pvArray) + (0 * ptLinkedList->i32ElementSize);
                memcpy(pvLinkedListElement, pvElement, ptLinkedList->i32ElementSize);
            }
        }
        else
        {
            i32ElementIndex1 = _getEndIndex(ptLinkedList);
            i32ElementIndex2 = _getValidIndex(ptLinkedList);
            ptLinkedList->ptElemHeaderArray[i32ElementIndex1].i32NextIndex = i32ElementIndex2;
            ptLinkedList->ptElemHeaderArray[i32ElementIndex2].i32NextIndex = -1;
            ptLinkedList->ptElemHeaderArray[i32ElementIndex2].i32PreviousIndex = i32ElementIndex1;
            ptLinkedList->ptElemHeaderArray[i32ElementIndex2].bValid = true;
            {
                pvLinkedListElement = ((uint8_t *)ptLinkedList->pvArray) + (i32ElementIndex2 * ptLinkedList->i32ElementSize);
                memcpy(pvLinkedListElement, pvElement, ptLinkedList->i32ElementSize);
            }
        }

        ptLinkedList->i32ActualLen += 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool LinkedList_remove(LinkedList_t *ptLinkedList, void *pvElement)
{
    int32_t i32RemovedElemIndex, i32ElementIndex1, i32ElementIndex2;

    if(!LinkedList_isEmpty(ptLinkedList))
    {
        i32RemovedElemIndex = LinkedList_find(ptLinkedList, pvElement);
        if(i32RemovedElemIndex == -1)
        {
            return false;
        }

        ptLinkedList->ptElemHeaderArray[i32RemovedElemIndex].bValid = false;
        i32ElementIndex1 = ptLinkedList->ptElemHeaderArray[i32RemovedElemIndex].i32PreviousIndex;
        i32ElementIndex2 = ptLinkedList->ptElemHeaderArray[i32RemovedElemIndex].i32NextIndex;

        if(i32ElementIndex1 != -1)
        {
            ptLinkedList->ptElemHeaderArray[i32ElementIndex1].i32NextIndex = i32ElementIndex2;
        }
        else
        {
            ptLinkedList->i32Head = i32ElementIndex2;
        }

        if(i32ElementIndex2 != -1)
        {
            ptLinkedList->ptElemHeaderArray[i32ElementIndex2].i32PreviousIndex = i32ElementIndex1;
        }

        ptLinkedList->i32ActualLen -= 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool LinkedList_insertHead(LinkedList_t *ptLinkedList, void *pvElement)
{
    int32_t i32ElementIndex;
    void *pvLinkedListElement;

    if(!LinkedList_isFull(ptLinkedList))
    {
        i32ElementIndex = _getValidIndex(ptLinkedList);
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].bValid = true;
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32PreviousIndex = -1;
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32NextIndex = ptLinkedList->i32Head;
        ptLinkedList->ptElemHeaderArray[ptLinkedList->i32Head].i32PreviousIndex = i32ElementIndex;
        ptLinkedList->i32Head = i32ElementIndex;
        {
            pvLinkedListElement = ((uint8_t *)ptLinkedList->pvArray) + (i32ElementIndex * ptLinkedList->i32ElementSize);
            memcpy(pvLinkedListElement, pvElement, ptLinkedList->i32ElementSize);
        }

        ptLinkedList->i32ActualLen += 1;
        return true;
    }
    else
    {
        return false;
    }
}

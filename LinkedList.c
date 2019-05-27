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

void LinkedList_allocateStatic(LinkedList_t *ptLinkedList, ElementHeader_t *ptElemHeaderArray, void *pvLinkedListArray, int32_t i32LinkedListLen, int32_t i32ElementSize)
{
    int32_t ii;

    ptLinkedList->i32ElementSize = i32ElementSize;
    ptLinkedList->i32Head = -1;
    ptLinkedList->i32End = -1;
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
    ptLinkedList->i32End = -1;
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

bool LinkedList_reallocateDynamic(LinkedList_t *ptLinkedList, int32_t i32NewLinkedListLen)
{
    void *pvData;
    ElementHeader_t *ptHeader;

    pvData = (void *)realloc(ptLinkedList->pvArray, i32NewLinkedListLen * ptLinkedList->i32ElementSize);
    ptHeader = (ElementHeader_t *)realloc(ptLinkedList->ptElemHeaderArray, i32NewLinkedListLen * sizeof(ElementHeader_t));

    if(pvData != NULL)
    {
        ptLinkedList->pvArray = pvData;
    }
    if(ptHeader != NULL)
    {
        ptLinkedList->ptElemHeaderArray = ptHeader;
    }

    if((pvData != NULL) && (ptHeader != NULL))
    {
        ptLinkedList->i32LinkedListLen = i32NewLinkedListLen;
        return true;
    }
    else
    {
        return false;
    }
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
    void *pvLinkedListElement;

    ptLinkedList->bHeadingToEnd = false;

    if(!LinkedList_isEmpty(ptLinkedList))
    {
        ptLinkedList->i32Next = ptLinkedList->ptElemHeaderArray[ptLinkedList->i32End].i32PreviousIndex;
        {
            pvLinkedListElement = ((uint8_t *)ptLinkedList->pvArray) + (ptLinkedList->i32End * ptLinkedList->i32ElementSize);
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
    int32_t i32ElementIndex;
    void *pvLinkedListElement;

    if(!LinkedList_isFull(ptLinkedList))
    {
        if(ptLinkedList->i32Head == -1)
        {
            ptLinkedList->i32Head = 0;
            ptLinkedList->i32End = 0;
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
            i32ElementIndex = _getValidIndex(ptLinkedList);
            ptLinkedList->ptElemHeaderArray[ptLinkedList->i32End].i32NextIndex = i32ElementIndex;
            ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32NextIndex = -1;
            ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32PreviousIndex = ptLinkedList->i32End;
            ptLinkedList->ptElemHeaderArray[i32ElementIndex].bValid = true;
            ptLinkedList->i32End = i32ElementIndex;
            {
                pvLinkedListElement = ((uint8_t *)ptLinkedList->pvArray) + (i32ElementIndex * ptLinkedList->i32ElementSize);
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
        else
        {
            ptLinkedList->i32End = i32ElementIndex1;
        }

        ptLinkedList->i32ActualLen -= 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool LinkedList_removeHead(LinkedList_t *ptLinkedList)
{
    int32_t i32NewHeadElemIndex;

    if(!LinkedList_isEmpty(ptLinkedList))
    {
        ptLinkedList->ptElemHeaderArray[ptLinkedList->i32Head].bValid = false;
        i32NewHeadElemIndex = ptLinkedList->ptElemHeaderArray[ptLinkedList->i32Head].i32NextIndex;

        if(i32NewHeadElemIndex == -1)
        {
            ptLinkedList->i32Head = -1;
            ptLinkedList->i32End = -1;
        }
        else
        {
            ptLinkedList->ptElemHeaderArray[i32NewHeadElemIndex].i32PreviousIndex = -1;
            ptLinkedList->i32Head = i32NewHeadElemIndex;
        }

        ptLinkedList->i32ActualLen -= 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool LinkedList_removeEnd(LinkedList_t *ptLinkedList)
{
    int32_t i32NewEndElemIndex;

    if(!LinkedList_isEmpty(ptLinkedList))
    {
        ptLinkedList->ptElemHeaderArray[ptLinkedList->i32End].bValid = false;
        i32NewEndElemIndex = ptLinkedList->ptElemHeaderArray[ptLinkedList->i32End].i32PreviousIndex;

        if(i32NewEndElemIndex == -1)
        {
            ptLinkedList->i32Head = -1;
            ptLinkedList->i32End = -1;
        }
        else
        {
            ptLinkedList->ptElemHeaderArray[i32NewEndElemIndex].i32NextIndex = -1;
            ptLinkedList->i32End = i32NewEndElemIndex;
        }

        ptLinkedList->i32ActualLen -= 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool LinkedList_insertAtHead(LinkedList_t *ptLinkedList, void *pvElement)
{
    int32_t i32ElementIndex;
    void *pvLinkedListElement;

    if(!LinkedList_isFull(ptLinkedList))
    {
        i32ElementIndex = _getValidIndex(ptLinkedList);
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].bValid = true;
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32PreviousIndex = -1;
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32NextIndex = ptLinkedList->i32Head;

        if(ptLinkedList->i32Head != -1)
        {
            ptLinkedList->ptElemHeaderArray[ptLinkedList->i32Head].i32PreviousIndex = i32ElementIndex;
        }
        else
        {
            ptLinkedList->i32End = i32ElementIndex;
        }

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

bool LinkedList_insertAtEnd(LinkedList_t *ptLinkedList, void *pvElement)
{
    int32_t i32ElementIndex;
    void *pvLinkedListElement;

    if(!LinkedList_isFull(ptLinkedList))
    {
        i32ElementIndex = _getValidIndex(ptLinkedList);
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].bValid = true;
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32PreviousIndex = ptLinkedList->i32End;
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32NextIndex = -1;

        if(ptLinkedList->i32End != -1)
        {
            ptLinkedList->ptElemHeaderArray[ptLinkedList->i32End].i32NextIndex = i32ElementIndex;
        }
        else
        {
            ptLinkedList->i32Head = i32ElementIndex;
        }

        ptLinkedList->i32End = i32ElementIndex;

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

bool LinkedList_insertAfter(LinkedList_t *ptLinkedList, void *pvElement, void *pvAfterThisElem)
{
    int32_t i32ElementIndex, i32AfterThisElemIndex, i32AfterNextElemIndex;
    void *pvLinkedListElement;

    if(!LinkedList_isFull(ptLinkedList))
    {
        i32AfterThisElemIndex = LinkedList_find(ptLinkedList, pvAfterThisElem);
        if(i32AfterThisElemIndex == -1)
        {
            return false;
        }

        i32ElementIndex = _getValidIndex(ptLinkedList);
        i32AfterNextElemIndex = ptLinkedList->ptElemHeaderArray[i32AfterThisElemIndex].i32NextIndex;

        ptLinkedList->ptElemHeaderArray[i32ElementIndex].bValid = true;
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32PreviousIndex = i32AfterThisElemIndex;
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32NextIndex = i32AfterNextElemIndex;
        ptLinkedList->ptElemHeaderArray[i32AfterThisElemIndex].i32NextIndex = i32ElementIndex;

        if(i32AfterNextElemIndex != -1)
        {
            ptLinkedList->ptElemHeaderArray[i32AfterNextElemIndex].i32PreviousIndex = i32ElementIndex;
        }
        else
        {
            ptLinkedList->i32End = i32ElementIndex;
        }

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

bool LinkedList_insertBefore(LinkedList_t *ptLinkedList, void *pvElement, void *pvBeforeThisElem)
{
    int32_t i32ElementIndex, i32BeforeThisElemIndex, i32BeforePreviousElemIndex;
    void *pvLinkedListElement;

    if(!LinkedList_isFull(ptLinkedList))
    {
        i32BeforeThisElemIndex = LinkedList_find(ptLinkedList, pvBeforeThisElem);
        if(i32BeforeThisElemIndex == -1)
        {
            return false;
        }

        i32ElementIndex = _getValidIndex(ptLinkedList);
        i32BeforePreviousElemIndex = ptLinkedList->ptElemHeaderArray[i32BeforeThisElemIndex].i32PreviousIndex;

        ptLinkedList->ptElemHeaderArray[i32ElementIndex].bValid = true;
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32NextIndex = i32BeforeThisElemIndex;
        ptLinkedList->ptElemHeaderArray[i32ElementIndex].i32PreviousIndex =  i32BeforePreviousElemIndex;
        ptLinkedList->ptElemHeaderArray[i32BeforeThisElemIndex].i32PreviousIndex = i32ElementIndex;
        if(i32BeforePreviousElemIndex != -1)
        {
            ptLinkedList->ptElemHeaderArray[i32BeforePreviousElemIndex].i32NextIndex = i32ElementIndex;
        }
        else
        {
            ptLinkedList->i32Head = i32ElementIndex;
        }

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

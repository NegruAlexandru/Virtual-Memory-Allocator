#include "lists_structs.h"

#pragma once

#ifndef ALLOCATOR_FUNCTIONS_H
#define ALLOCATOR_FUNCTIONS_H

arrayOfLists_t *createArrayOfListsWithIncreasingSize(long address, int number, int memorySize);
void printMemoryDump(arrayOfLists_t *arrayOfListsFreeMemory, arrayOfLists_t *arrayOfListsAllocatedMemory, int nrOfMallocs, int nrOfFrees, int nrOfFragmentations);
node_t *getIdealBlock(arrayOfLists_t *arrayOfLists, int sizeNeeded, int *sizeOfBlock);
node_t *getAvailableBlock(arrayOfLists_t *arrayOfLists, int amountOfLists, int sizeNeeded, int *sizeOfBlock);
void moveBlockToArrayOfLists(arrayOfLists_t *arrayOfLists, node_t *block, int size);
int mallocFunction(arrayOfLists_t *arrayOfListsFreeMemory, arrayOfLists_t *arrayOfListsAllocatedMemory, int nrOfFragmentations);
int freeBlock(arrayOfLists_t *arrayOfListsAllocatedMemory, arrayOfLists_t *arrayOfListsFreeMemory, long address, int freeMode);

#endif

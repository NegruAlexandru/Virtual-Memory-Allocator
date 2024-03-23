#include "lists_structs.h"

#pragma once

#ifndef ALLOCATOR_FUNCTIONS_H
#define ALLOCATOR_FUNCTIONS_H

arrayOfLists_t *createArrayOfListsWithIncreasingSize(long address, int number, int memorySize);
void printMemoryDump(arrayOfLists_t *arrayOfListsFreeMemory, arrayOfLists_t *arrayOfListsAllocatedMemory, int nrOfMallocs, int nrOfFrees, int nrOfFragmentations);
node_t *getIdealBlock(arrayOfLists_t *arrayOfLists, int amountOfLists, int sizeNeeded, int *sizeOfBlock);
node_t *getAvailableBlock(arrayOfLists_t *arrayOfLists, int amountOfLists, int sizeNeeded, int *sizeOfBlock);
void moveBlockToArrayList(arrayOfLists_t *arrayOfListsAllocatedMemory, node_t *block, int size);
int mallocFunction(arrayOfLists_t *arrayOfListsFreeMemory, arrayOfLists_t *arrayOfListsAllocatedMemory);

#endif

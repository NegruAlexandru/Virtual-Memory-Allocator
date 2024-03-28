#include "lists_structs.h"

#pragma once

#ifndef ALLOCATOR_FUNCTIONS_H
#define ALLOCATOR_FUNCTIONS_H

array_of_lists_t *createArrayOfListsWithIncreasingSize(long address, int number, int memorySize);
void printMemoryDump(array_of_lists_t *arrayOfListsFreeMemory, array_of_lists_t *arrayOfListsAllocatedMemory, int nrOfMallocs, int nrOfFrees, int nrOfFragmentations);
node_t *getIdealBlock(array_of_lists_t *arrayOfLists, int sizeNeeded, int *sizeOfBlock);
node_t *getAvailableBlock(array_of_lists_t *array_lists, int size_needed, int *size_block);
void moveBlockToArrayOfLists(array_of_lists_t *arrayOfLists, node_t *block, int size);
int mallocFunction(array_of_lists_t *arrayOfListsFreeMemory, array_of_lists_t *arrayOfListsAllocatedMemory, int nrOfFragmentations, long *fragment_group);
int freeBlock(array_of_lists_t *arrayOfListsAllocatedMemory, array_of_lists_t *arrayOfListsFreeMemory, long address, int freeMode);
void deleteArrayOfLists(array_of_lists_t *arrayOfLists);
int isSpaceToWrite(array_of_lists_t *arrayOfLists, long address, int sizeToWrite);
void writeToAllocatedMemory(array_of_lists_t *arrayOfLists, char *data, long address, int sizeToWrite);
int isRequestedMemoryAllocated(array_of_lists_t *arrayOfLists, long address, int sizeToRead);
char *readFromAllocatedMemory(array_of_lists_t *arrayOfLists, long address, int sizeToRead);

#endif

#include "lists_structs.h"

#pragma once

#ifndef ALLOCATOR_FUNCTIONS_H
#define ALLOCATOR_FUNCTIONS_H

arrayOfLists_t *createArrayOfListsWithIncreasingSize(long address, int number, int memorySize);
void printMemoryDump(arrayOfLists_t *arrayOfListsFreeMemory, arrayOfLists_t *arrayOfListsAllocatedMemory);
node_t *searchForBlock(arrayOfLists_t *arrayOfLists, int amountOfLists, int sizeNeeded);

#endif

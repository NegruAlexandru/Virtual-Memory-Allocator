#include "lists_structs.h"

#pragma once

#ifndef ALLOCATOR_FUNCTIONS_H
#define ALLOCATOR_FUNCTIONS_H

doublyLinkedList_t **createArrayOfListsWithIncreasingSize(int address, int number, int memory_size);
void printMemoryDump(doublyLinkedList_t **arrayOfLists, int number, int memory_size);
#endif

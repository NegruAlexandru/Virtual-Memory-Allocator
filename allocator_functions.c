#include "allocator_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include "lists_structs.h"
#include "lists_functions.h"
#include "datatype_functions.h"

arrayOfLists_t *createArrayOfListsWithIncreasingSize(long address, int number, int memorySize) {
	arrayOfLists_t *arr = (arrayOfLists_t *)malloc(sizeof(arrayOfLists_t));

	arr->lists = (doublyLinkedList_t **)malloc(number * sizeof(doublyLinkedList_t *));

	arr->number = number;

	arr->memorySize = memorySize;

	int dataSize = 8;

	for (int i = 0; i < number; i++) {
		arr->lists[i] = createDoublyLinkedList(dataSize);

		for (int j = 0; j < memorySize / dataSize; j++) {
			int n = 0;

			addToNthPosition(arr->lists[i], j, &n, address + j * dataSize + i * memorySize);
			printf("0x%s\n", base10to16(address + j * dataSize + i * memorySize));
		}

		dataSize *= 2;
	}

	return arr;
}

void printMemoryDump(arrayOfLists_t *arrayOfListsFreeMemory, arrayOfLists_t *arrayOfListsAllocatedMemory) {
	int numberListsFree = arrayOfListsFreeMemory->number;
	int memorySizeFree = arrayOfListsFreeMemory->memorySize;

	int numberListsAllocated = arrayOfListsAllocatedMemory->number;
	int memorySizeAllocated = arrayOfListsAllocatedMemory->memorySize;

	printf("+++++DUMP+++++\n");
	printf("Total memory: %d\n", numberListsFree * memorySizeFree);
	printf("Total allocated memory: %d\n", numberListsAllocated * memorySizeAllocated);
	printf("Total free memory: %d\n", numberListsFree * memorySizeFree - numberListsAllocated * memorySizeAllocated);

	int freeBlocks = 0;
	for (int i = 0; i < arrayOfListsFreeMemory->number; i++) {
		freeBlocks += arrayOfListsFreeMemory->lists[i]->size;
	}
	printf("Free blocks: %d\n", freeBlocks);

	int allocatedBlocks = 0;
	for (int i = 0; i < arrayOfListsAllocatedMemory->number; i++) {
		allocatedBlocks += arrayOfListsAllocatedMemory->lists[i]->size;
	}
	printf("Number of allocated blocks: %d\n", allocatedBlocks);

	printf("Number of malloc calls: %d\n", 0);
	printf("Number of fragmentations: %d\n", 0);
	printf("Number of free calls: %d\n", 0);

	for (int i = 0; i < arrayOfListsFreeMemory->number; i++) {
		int dataSize = arrayOfListsFreeMemory->lists[i]->dataSize;
		int size = arrayOfListsFreeMemory->lists[i]->size;

		printf("Blocks with %d bytes - %d free block(s) : ", dataSize, size);

		node_t *current = arrayOfListsFreeMemory->lists[i]->head;

		for (int j = 0; j < size; j++) {
			printf("0x%lx ", current->address);
			current = current->next;
		}
		printf("\n");
	}
	printf("Allocated blocks: %d\n", 0);

}

node_t *searchForBlock(arrayOfLists_t *arrayOfLists, int amountOfLists, int sizeNeeded) {
	for (int i = 0; i < amountOfLists; i++) {
		if (arrayOfLists->lists[i]->dataSize == sizeNeeded) {
			node_t *current = arrayOfLists->lists[i]->head;
			while (current != NULL) {
				if (*(int *)current->data == 0) {
					return current;
				}
				current = current->next;
			}
		} else if (arrayOfLists->lists[i]->dataSize > sizeNeeded) {
			node_t *current = arrayOfLists->lists[i]->head;
			while (current != NULL) {
				if (*(int *)current->data == 0) {
					return current;
				}
				current = current->next;
			}
			// TODO: split the block
		}
		i++;
	}
	return NULL;
}
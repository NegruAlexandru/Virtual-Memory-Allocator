#include "allocator_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

			createNodeNthPosition(arr->lists[i], j, &n, address + j * dataSize + i * memorySize);
		}

		dataSize *= 2;
	}

	return arr;
}

void printMemoryDump(arrayOfLists_t *arrayOfListsFreeMemory, arrayOfLists_t *arrayOfListsAllocatedMemory, int nrOfMallocs, int nrOfFrees, int nrOfFragmentations) {
	int totalAllocatedMemory = 0;
	int totalFreeMemory = 0;

	bubbleSortArrayOfListsBySize(arrayOfListsFreeMemory->lists, arrayOfListsFreeMemory->number);
	bubbleSortArrayOfListsBySize(arrayOfListsAllocatedMemory->lists, arrayOfListsAllocatedMemory->number);

	for (int i = 0; i < arrayOfListsFreeMemory->number; i++) {
		totalFreeMemory += arrayOfListsFreeMemory->lists[i]->size * arrayOfListsFreeMemory->lists[i]->dataSize;
	}

	for (int i = 0; i < arrayOfListsAllocatedMemory->number; i++) {
		totalAllocatedMemory += arrayOfListsAllocatedMemory->lists[i]->size * arrayOfListsAllocatedMemory->lists[i]->dataSize;
	}

	printf("+++++DUMP+++++\n");
	printf("Total memory: %d bytes\n", totalFreeMemory + totalAllocatedMemory);
	printf("Total allocated memory: %d bytes\n", totalAllocatedMemory);
	printf("Total free memory: %d bytes\n", totalFreeMemory);

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

	printf("Number of malloc calls: %d\n", nrOfMallocs);
	printf("Number of fragmentations: %d\n", nrOfFragmentations);
	printf("Number of free calls: %d\n", nrOfFrees);

	for (int i = 0; i < arrayOfListsFreeMemory->number; i++) {
		if (arrayOfListsFreeMemory->lists[i]->size == 0) {
			continue;
		}
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

	printf("Allocated blocks : ");

	for (int i = 0; i < arrayOfListsAllocatedMemory->number; i++) {
		node_t *current = arrayOfListsAllocatedMemory->lists[i]->head;

		while (current != NULL) {
			printf("(0x%lx - %d) ", current->address, arrayOfListsAllocatedMemory->lists[i]->dataSize);
			current = current->next;
		}
	}

	printf("\n");

	printf("-----DUMP----\n");
}

node_t *getIdealBlock(arrayOfLists_t *arrayOfLists, int sizeNeeded, int *sizeOfBlock) {
	for (int i = 0; i < arrayOfLists->number; i++) {
		if (arrayOfLists->lists[i]->dataSize == sizeNeeded) {
			*sizeOfBlock = arrayOfLists->lists[i]->dataSize;
			bubbleSortListByAddress(arrayOfLists->lists[i]);
			node_t *block = removeNthPosition(arrayOfLists->lists[i], 0);
			return block;
		}
	}
	return NULL;
}

node_t *getAvailableBlock(arrayOfLists_t *arrayOfLists, int amountOfLists, int sizeNeeded, int *sizeOfBlock) {
	int position = -1;
	int minAddress;

	for (int i = 0; i < amountOfLists; i++) {
		if (arrayOfLists->lists[i]->dataSize >= sizeNeeded) {
			bubbleSortListByAddress(arrayOfLists->lists[i]);

			if (position == -1) {
				position = i;
				minAddress = arrayOfLists->lists[i]->head->address;
			} else {
				if (arrayOfLists->lists[i]->head->address < minAddress) {
					position = i;
					minAddress = arrayOfLists->lists[i]->head->address;
				}
			}
		}
	}

	if (position == -1) {
		return NULL;
	}

	*sizeOfBlock = arrayOfLists->lists[position]->dataSize;
	bubbleSortListByAddress(arrayOfLists->lists[position]);
	node_t *block = removeNthPosition(arrayOfLists->lists[position], 0);
	return block;
}

void moveBlockToArrayOfLists(arrayOfLists_t *arrayOfLists, node_t *block, int size) {
	for (int i = 0; i < arrayOfLists->number; i++) {
		if (arrayOfLists->lists[i]->dataSize == size) {
			addNodeToNthPosition(arrayOfLists->lists[i], block, arrayOfLists->lists[i]->size);
			return;
		}
	}

	doublyLinkedList_t **tmp = (doublyLinkedList_t **)realloc(arrayOfLists->lists, (arrayOfLists->number + 1) * sizeof(doublyLinkedList_t *));

	if (!tmp) {
		printf("Realloc fail\n");
		return;
	}

	arrayOfLists->lists = tmp;

	arrayOfLists->lists[arrayOfLists->number] = createDoublyLinkedList(size);

	arrayOfLists->number++;

	addNodeToNthPosition(arrayOfLists->lists[arrayOfLists->number - 1], block, 0);
}

int mallocFunction(arrayOfLists_t *arrayOfListsFreeMemory, arrayOfLists_t *arrayOfListsAllocatedMemory, int nrOfFragmentations) {
	//requested size
	int sizeNeeded;
	scanf("%d", &sizeNeeded);

	//modified by getBlock functions
	int sizeOfBlock = 0;

	//searching for free block
	node_t *block = getIdealBlock(arrayOfListsFreeMemory, sizeNeeded, &sizeOfBlock);

	if (!block) {
		block = getAvailableBlock(arrayOfListsFreeMemory, arrayOfListsFreeMemory->number, sizeNeeded, &sizeOfBlock);

		if (!block) {
			printf("Out of memory\n");
			return -1;
		}
	}

	//if block is exactly the size needed
	if (sizeOfBlock == sizeNeeded) {
		// block being moved to allocated memory
		moveBlockToArrayOfLists(arrayOfListsAllocatedMemory, block, sizeOfBlock);

		bubbleSortListByAddress(getListWithSize(arrayOfListsAllocatedMemory, sizeOfBlock));
		bubbleSortArrayOfListsBySize(arrayOfListsAllocatedMemory->lists, arrayOfListsAllocatedMemory->number);

		return 0;
	}

	//if block is bigger than the size needed
	if (sizeOfBlock > sizeNeeded) {
		//requested block being moved to allocated memory
		node_t *requestedBlock;
		if (block->origin >= 0) {
			requestedBlock = createEmptyNodeWithOrigin(sizeNeeded, block->address, block->origin);
		} else {
			requestedBlock = createEmptyNodeWithOrigin(sizeNeeded, block->address, nrOfFragmentations);
		}

		moveBlockToArrayOfLists(arrayOfListsAllocatedMemory, requestedBlock, sizeNeeded);

		bubbleSortListByAddress(getListWithSize(arrayOfListsAllocatedMemory, sizeNeeded));

		//remaining block being moved to free memory
		node_t *remainingBlock;
		if (block->origin >= 0) {
			remainingBlock = createEmptyNodeWithOrigin(sizeOfBlock - sizeNeeded, block->address + sizeNeeded, block->origin);
		} else {
			remainingBlock = createEmptyNodeWithOrigin(sizeOfBlock - sizeNeeded, block->address + sizeNeeded, nrOfFragmentations);
		}

		moveBlockToArrayOfLists(arrayOfListsFreeMemory, remainingBlock, sizeOfBlock - sizeNeeded);

		bubbleSortListByAddress(getListWithSize(arrayOfListsFreeMemory, sizeOfBlock - sizeNeeded));

		free(block->data);
		free(block);

		return 1;
	}

	return -1;
}

int freeBlock(arrayOfLists_t *arrayOfListsAllocatedMemory, arrayOfLists_t *arrayOfListsFreeMemory, long address, int freeMode) {
	int blockSize = 0;

	node_t *block = removeNAddress(arrayOfListsAllocatedMemory, address, &blockSize);

	if (!block) {
		printf("Invalid free\n");
		return -1;
	}

	if (freeMode == 0) {
		moveBlockToArrayOfLists(arrayOfListsFreeMemory, block, blockSize);
		bubbleSortListByAddress(getListWithSize(arrayOfListsFreeMemory, blockSize));

		return 0;

	} else if (freeMode == 1) {
		if (block->origin >= 0) {

			int neighbourSize = 0;

			node_t *neighbour = removeNeighbourNode(block, arrayOfListsFreeMemory, &blockSize, &neighbourSize);

			while (neighbour) {
				if (neighbour->origin == block->origin) {
					blockSize += neighbourSize;

					if (block->address < neighbour->address) {
						void *tmp = (void *)realloc(block->data, blockSize);

						if (!tmp) {
							printf("Realloc fail\n");
							return -1;
						}

						block->data = tmp;

						free(neighbour->data);
						free(neighbour);
					} else {
						void *tmp = (void *)realloc(neighbour->data, blockSize);

						if (!tmp) {
							printf("Realloc fail\n");
							return -1;
						}

						neighbour->data = tmp;

						free(block->data);
						free(block);

						block = neighbour;
						neighbour = NULL;
					}
				}

				neighbour = removeNeighbourNode(block, arrayOfListsFreeMemory, &blockSize, &neighbourSize);
			}

			moveBlockToArrayOfLists(arrayOfListsFreeMemory, block, blockSize);

			bubbleSortListByAddress(getListWithSize(arrayOfListsFreeMemory, blockSize));

			return 0;
		}
	}
}

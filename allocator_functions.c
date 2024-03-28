#include "allocator_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lists_structs.h"
#include "lists_functions.h"
#include "datatype_functions.h"

array_of_lists_t *createArrayOfListsWithIncreasingSize(long address, int number, int memorySize) {
	array_of_lists_t *arr = (array_of_lists_t *)malloc(sizeof(array_of_lists_t));

	arr->lists = (doublyLinkedList_t **)malloc(number * sizeof(doublyLinkedList_t *));

	arr->number = number;

	arr->memorySize = memorySize;

	int dataSize = 8;

	for (int i = 0; i < number; i++) {
		arr->lists[i] = createDoublyLinkedList(dataSize);

		for (int j = 0; j < memorySize / dataSize; j++) {
			createNodeNthPosition(arr->lists[i], j, address + j * dataSize + i * memorySize);
		}

		dataSize *= 2;
	}

	return arr;
}

void printMemoryDump(array_of_lists_t *arrayOfListsFreeMemory, array_of_lists_t *arrayOfListsAllocatedMemory, int nrOfMallocs, int nrOfFrees, int nrOfFragmentations) {
	int totalAllocatedMemory = 0;
	int totalFreeMemory = 0;

	bubbleSortArrayOfListsBySize(arrayOfListsAllocatedMemory);
	bubbleSortArrayOfListsBySize(arrayOfListsFreeMemory);

	for (int i = 0; i < arrayOfListsFreeMemory->number; i++) {
		totalFreeMemory += arrayOfListsFreeMemory->lists[i]->size * arrayOfListsFreeMemory->lists[i]->data_size;
	}

	for (int i = 0; i < arrayOfListsAllocatedMemory->number; i++) {
		totalAllocatedMemory += arrayOfListsAllocatedMemory->lists[i]->size * arrayOfListsAllocatedMemory->lists[i]->data_size;
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
		int dataSize = arrayOfListsFreeMemory->lists[i]->data_size;
		int size = arrayOfListsFreeMemory->lists[i]->size;

		printf("Blocks with %d bytes - %d free block(s) :", dataSize, size);

		node_t *current = arrayOfListsFreeMemory->lists[i]->head;

		for (int j = 0; j < size; j++) {
			printf(" 0x%lx", current->address);
			current = current->next;
		}
		printf("\n");
	}

	printf("Allocated blocks :");

	long address = -1;

	//print allocated blocks sorted by address increasing
	for (int i = 0; i < allocatedBlocks; i++) {
		long minAddress = 0xfffffff;

		for (int j = 0; j < arrayOfListsAllocatedMemory->number; j++) {
			if (arrayOfListsAllocatedMemory->lists[j]->size == 0) {
				continue;
			}

			node_t *current = arrayOfListsAllocatedMemory->lists[j]->head;

			for (int k = 0; k < arrayOfListsAllocatedMemory->lists[j]->size; k++) {
				if (current->address < minAddress && current->address > address) {
					minAddress = current->address;
				}
				current = current->next;
			}
		}

		address = minAddress;
		printf(" (0x%lx - %d)", address, getSizeOfBlockByAddress(arrayOfListsAllocatedMemory, address));
	}

	printf("\n");

	printf("-----DUMP-----\n");
}

node_t *getIdealBlock(array_of_lists_t *arrayOfLists, int sizeNeeded, int *sizeOfBlock) {
	for (int i = 0; i < arrayOfLists->number; i++) {
		if (arrayOfLists->lists[i]->data_size == sizeNeeded) {
			if (arrayOfLists->lists[i]->size == 0) {
				continue;
			}
			*sizeOfBlock = arrayOfLists->lists[i]->data_size;
			sort_by_address(arrayOfLists->lists[i]);
			node_t *block = remove_nth_position(arrayOfLists->lists[i], 0);
			return block;
		}
	}
	return NULL;
}

node_t *getAvailableBlock(array_of_lists_t *array_lists, int size_needed, int *size_block) {
	int position = -1;

	for (int i = 0; i < array_lists->number; i++) {
		if (array_lists->lists[i]->data_size > size_needed) {
			if (array_lists->lists[i]->size == 0) {
				continue;
			}
			sort_by_address(array_lists->lists[i]);

			position = i;

			break;
		}
	}

	if (position == -1) {
		return NULL;
	}

	*size_block = array_lists->lists[position]->data_size;
	sort_by_address(array_lists->lists[position]);
	node_t *block = remove_nth_position(array_lists->lists[position], 0);
	return block;
}

void moveBlockToArrayOfLists(array_of_lists_t *arrayOfLists, node_t *block, int size) {
	for (int i = 0; i < arrayOfLists->number; i++) {
		if (arrayOfLists->lists[i]->data_size == size) {
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

	addNodeToNthPosition(arrayOfLists->lists[arrayOfLists->number], block, 0);

	arrayOfLists->number++;
}

int mallocFunction(array_of_lists_t *arrayOfListsFreeMemory, array_of_lists_t *arrayOfListsAllocatedMemory, int nrOfFragmentations, long *fragment_group) {
	//requested size
	int sizeNeeded;
	scanf("%d", &sizeNeeded);

	//modified by getBlock functions
	int sizeOfBlock = 0;

	//searching for free block
	node_t *block = getIdealBlock(arrayOfListsFreeMemory, sizeNeeded, &sizeOfBlock);

	if (!block) {
		block = getAvailableBlock(arrayOfListsFreeMemory, sizeNeeded, &sizeOfBlock);

		if (!block) {
			printf("Out of memory\n");
			return -1;
		}
	}

	//if block is exactly the size needed
	if (sizeOfBlock == sizeNeeded) {
		// block being moved to allocated memory
		moveBlockToArrayOfLists(arrayOfListsAllocatedMemory, block, sizeOfBlock);

		sort_by_address(getListWithSize(arrayOfListsAllocatedMemory, sizeOfBlock));
		bubbleSortArrayOfListsBySize(arrayOfListsAllocatedMemory);

		return 0;
	}

	//if block is bigger than the size needed
	if (sizeOfBlock > sizeNeeded) {


		node_t *requestedBlock;
		node_t *remainingBlock;

		if (block->origin >= 0) {
			requestedBlock = createEmptyNodeWithOrigin(sizeNeeded, block->address, block->origin);
			remainingBlock = createEmptyNodeWithOrigin(sizeOfBlock - sizeNeeded, block->address + sizeNeeded, block->origin);
		} else {
			requestedBlock = createEmptyNodeWithOrigin(sizeNeeded, block->address, *fragment_group);
			remainingBlock = createEmptyNodeWithOrigin(sizeOfBlock - sizeNeeded, block->address + sizeNeeded, *fragment_group);
			(*fragment_group) = (*fragment_group) + 1;
		}

		moveBlockToArrayOfLists(arrayOfListsAllocatedMemory, requestedBlock, sizeNeeded);
		sort_by_address(getListWithSize(arrayOfListsAllocatedMemory, sizeNeeded));
		bubbleSortArrayOfListsBySize(arrayOfListsAllocatedMemory);

		moveBlockToArrayOfLists(arrayOfListsFreeMemory, remainingBlock, sizeOfBlock - sizeNeeded);
		sort_by_address(getListWithSize(arrayOfListsFreeMemory, sizeOfBlock - sizeNeeded));
		bubbleSortArrayOfListsBySize(arrayOfListsFreeMemory);

		free(block->data);
		free(block);

		return 1;
	}

	return -1;
}

int freeBlock(array_of_lists_t *arrayOfListsAllocatedMemory, array_of_lists_t *arrayOfListsFreeMemory, long address, int freeMode) {
	int blockSize = 0;

	node_t *block = removeNAddress(arrayOfListsAllocatedMemory, address, &blockSize);

	if (!block) {
		printf("Invalid free\n");
		return -1;
	}

	if (freeMode == 0) {
		moveBlockToArrayOfLists(arrayOfListsFreeMemory, block, blockSize);
		sort_by_address(getListWithSize(arrayOfListsFreeMemory, blockSize));

		return 0;

	} else if (freeMode == 1) {
		if (block->origin >= 0) {

			int neighbourSize = 0;

			node_t *neighbour = removeNeighbourNode(block, arrayOfListsFreeMemory, &blockSize, &neighbourSize);

			while (neighbour) {
				blockSize += neighbourSize;

				if (block->address < neighbour->address) {
					void *tmp = realloc(block->data, blockSize);

					if (!tmp) {
						printf("Realloc fail\n");
						return -1;
					}

					block->data = tmp;

					free(neighbour->data);
					free(neighbour);
				} else {
					void *tmp = realloc(block->data, blockSize);

					if (!tmp) {
						printf("Realloc fail\n");
						return -1;
					}

					block->data = tmp;
					block->address = neighbour->address;

					free(neighbour->data);
					free(neighbour);
				}

				neighbour = removeNeighbourNode(block, arrayOfListsFreeMemory, &blockSize, &neighbourSize);
			}

			moveBlockToArrayOfLists(arrayOfListsFreeMemory, block, blockSize);
			sort_by_address(getListWithSize(arrayOfListsFreeMemory, blockSize));

			return 0;
		}

		moveBlockToArrayOfLists(arrayOfListsFreeMemory, block, blockSize);
		sort_by_address(getListWithSize(arrayOfListsFreeMemory, blockSize));

		return 0;
	}

	return -1;
}

void deleteArrayOfLists(array_of_lists_t *arrayOfLists) {
	for (int i = 0; i < arrayOfLists->number; i++) {
		node_t *current = arrayOfLists->lists[i]->head;

		while (current != NULL) {
			free(current->data);
			node_t *tmp = current;
			current = current->next;
			free(tmp);
		}

		free(arrayOfLists->lists[i]);
	}

	free(arrayOfLists->lists);
	free(arrayOfLists);
}

int isSpaceToWrite(array_of_lists_t *arrayOfLists, long address, int sizeToWrite) {
	while (sizeToWrite > 0) {
		int size = getSizeOfPartialBlockByAddress(arrayOfLists, address);

		if (!size) {
			return 0;
		}

		if (size >= sizeToWrite) {
			return 1;
		}

		address += size;
		sizeToWrite -= size;
	}

	return 1;
}

void writeToAllocatedMemory(array_of_lists_t *arrayOfLists, char *data, long address, int sizeToWrite) {
	while (sizeToWrite > 0) {
		int size = getSizeOfPartialBlockByAddress(arrayOfLists, address);

		node_t *block = getNodeByAddress(arrayOfLists, address);

		if (size >= sizeToWrite) {
			memcpy(block->data, data, sizeToWrite);
			return;
		}

		memcpy(block->data, data, size);

		data += size;
		address += size;
		sizeToWrite -= size;
	}
}

int isRequestedMemoryAllocated(array_of_lists_t *arrayOfLists, long address, int sizeToRead) {
	while (sizeToRead > 0) {
		int size = getSizeOfPartialBlockByAddress(arrayOfLists, address);
		if (!size) {
			return 0;
		}

		if (size >= sizeToRead) {
			return 1;
		}

		address += size;
		sizeToRead -= size;
	}

	return 1;
}

char *readFromAllocatedMemory(array_of_lists_t *arrayOfLists, long address, int sizeToRead) {
	char *data = (char *)malloc(sizeToRead * sizeof(char));
	if (!data) {
		printf("Malloc fail\n");
		return NULL;
	}

	char *dataStart = data;

	while (sizeToRead > 0) {
		int size = getSizeOfPartialBlockByAddress(arrayOfLists, address);

		node_t *block = getNodeByAddress(arrayOfLists, address);

		if (size >= sizeToRead) {
			memcpy(data, block->data, sizeToRead);
			return dataStart;
		}

		memcpy(data, block->data, size);
		data += size;
		address += size;
		sizeToRead -= size;
	}

	return dataStart;
}
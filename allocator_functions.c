#include "allocator_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include "lists_structs.h"
#include "lists_functions.h"
#include "datatype_functions.h"

doublyLinkedList_t **createArrayOfListsWithIncreasingSize(int address, int number, int memory_size) {
	doublyLinkedList_t **arr = malloc(number * sizeof(doublyLinkedList_t *));

	int size = 8;

	for (int i = 0; i < number; i++) {
		arr[i] = createDoublyLinkedList(size);

		for (int j = 0; j < memory_size / size; j++) {
			addNewEmptyNode(arr[i], j, address + j * size + i * memory_size);
			printf("0x%s\n", base10to16(address + j * size + i * memory_size));
		}

		size *= 2;
	}

	return arr;
}

void printMemoryDump(doublyLinkedList_t **arrayOfLists, int number, int memory_size) {
	printf("+++++DUMP+++++\n");
	printf("Total memory: %d\n", number * memory_size);
	printf("Total allocated memory: %d\n", 0);
	printf("Total free memory: %d\n", number * memory_size);

	int freeBlocks = 0;

	for (int i = 0; i < number; i++) {
		freeBlocks += arrayOfLists[i]->size;
	}

	printf("Number of free blocks: %d\n", freeBlocks);
	printf("Number of allocated blocks: %d\n", 0);
	printf("Number of malloc calls: %d\n", 0);
	printf("Number of fragmentations: %d\n", 0);
	printf("Number of free calls: %d\n", 0);
	for (int i = 0; i < number; i++) {
		printf("Blocks with %d bytes - %d free block(s) : ", 2 << (i + 2), 0);
		for (int j = 0; j < 5; j++) {
			printf("0x%x ", 777);
		}
		printf("\n");
	}
	printf("Allocated blocks: %d\n", 0);

}
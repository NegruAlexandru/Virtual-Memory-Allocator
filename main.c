#include <stdio.h>
#include <stdlib.h>
#include "lists_structs.h"
#include "lists_functions.h"
#include "allocator_functions.h"
#include "datatype_functions.h"
#include <string.h>

#define MAX_INPUT_SIZE 600
#define MAX_ADDRESS_SIZE 8

int main() {

	arrayOfLists_t *arrayOfListsFreeMemory = NULL, *arrayOfListsAllocatedMemory = NULL;

	int nrOfListsWithFreeMemory = 0, memorySize = 0;

	int nrOfMallocs = 0, nrOfFrees = 0, nrOfFragmentations = 0;

	while (1) {
		char choice[MAX_INPUT_SIZE];
		scanf("%s", choice);

		if (strcmp(choice, "INIT_HEAP") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));

			scanf("%s %d %d", address, &nrOfListsWithFreeMemory, &memorySize);

			int addressLong = base16to10(removeHexaPrefix(address));

			free(address);

			arrayOfListsFreeMemory = createArrayOfListsWithIncreasingSize(
					addressLong, nrOfListsWithFreeMemory, memorySize);

			arrayOfListsAllocatedMemory = (arrayOfLists_t *) malloc(sizeof(arrayOfLists_t));

			arrayOfListsAllocatedMemory->number = 0;

			arrayOfListsAllocatedMemory->memorySize = 0;

			arrayOfListsAllocatedMemory->lists = (doublyLinkedList_t **) malloc( 0);


		} else if (strcmp(choice, "MALLOC") == 0) {
			int mallocReturn = mallocFunction(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory);

			if (mallocReturn == 0) {
				nrOfMallocs++;
			} else if (mallocReturn == 1) {
				nrOfMallocs++;
				nrOfFragmentations++;
			}

		} else if (strcmp(choice, "FREE") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));
			scanf("%s", address);

			long addressLong = base16to10(removeHexaPrefix(address));

			free(address);

			//freeBlock(arrayOfListsAllocatedMemory, addressLong);

		} else if (strcmp(choice, "DUMP_MEMORY") == 0) {
			printMemoryDump(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory, nrOfMallocs, nrOfFrees, nrOfFragmentations);
		} else if (strcmp(choice, "exit") == 0) {
			break;
		} else {
			printf("Invalid command\n");
		}
	}

	return 0;
}

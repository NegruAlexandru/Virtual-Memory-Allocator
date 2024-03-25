#include <stdio.h>
#include <stdlib.h>
#include "lists_structs.h"
#include "lists_functions.h"
#include "allocator_functions.h"
#include "datatype_functions.h"
#include <string.h>

#define MAX_INPUT_SIZE 600
#define MAX_ADDRESS_SIZE 10

int main() {

	arrayOfLists_t *arrayOfListsFreeMemory = NULL, *arrayOfListsAllocatedMemory = NULL;

	int nrOfMallocs = 0, nrOfFrees = 0, nrOfFragmentations = 0;

	int freeMode = 0;

	while (1) {
		char choice[MAX_INPUT_SIZE];
		scanf("%s", choice);

		if (strcmp(choice, "INIT_HEAP") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));

			int nrOfListsWithFreeMemory = 0, memorySize = 0;

			scanf("%s %d %d %d", address, &nrOfListsWithFreeMemory, &memorySize, &freeMode);

			long addressLong = base16to10(removeHexaPrefix(address));

			arrayOfListsFreeMemory = createArrayOfListsWithIncreasingSize(
					addressLong, nrOfListsWithFreeMemory, memorySize);

			arrayOfListsAllocatedMemory = (arrayOfLists_t *) malloc(sizeof(arrayOfLists_t));

			arrayOfListsAllocatedMemory->number = 0;

			arrayOfListsAllocatedMemory->memorySize = 0;

			arrayOfListsAllocatedMemory->lists = (doublyLinkedList_t **) malloc( 0);

		} else if (strcmp(choice, "MALLOC") == 0) {
			int mallocReturn = mallocFunction(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory, nrOfFragmentations);

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

			if (addressLong != 0) {
				int freeReturn = freeBlock(arrayOfListsAllocatedMemory, arrayOfListsFreeMemory, addressLong, freeMode);

				if (freeReturn == 0) {
					nrOfFrees++;
				}
			}
		} else if (strcmp(choice, "WRITE") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));
			char *data = (char *) malloc(100 * sizeof(char));
			int sizeToWrite = 0;

			scanf("%s %s %d", address, data, &sizeToWrite);

			data = removeDoubleQuotes(data);

			if (sizeToWrite > strlen(data)) {
				sizeToWrite = strlen(data);
			}

			long addressLong = base16to10(removeHexaPrefix(address));

			if (isSpaceToWrite(arrayOfListsAllocatedMemory, addressLong, sizeToWrite)) {
				writeToAllocatedMemory(arrayOfListsAllocatedMemory, data, addressLong, sizeToWrite);
			} else {
				printf("Segmentation fault (core dumped)\n");
				printMemoryDump(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory, nrOfMallocs, nrOfFrees, nrOfFragmentations);
			}

		} else if (strcmp(choice, "READ") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));
			int sizeToRead = 0;

			scanf("%s %d", address, &sizeToRead);

			long addressLong = base16to10(removeHexaPrefix(address));

			if (isRequestedMemoryAllocated(arrayOfListsAllocatedMemory, addressLong, sizeToRead)) {
				char *data = readFromAllocatedMemory(arrayOfListsAllocatedMemory, addressLong, sizeToRead);
				printf("%s\n", data);

				free(data);
			} else {
				printf("Segmentation fault (core dumped)\n");
				printMemoryDump(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory, nrOfMallocs, nrOfFrees, nrOfFragmentations);
			}

		} else if (strcmp(choice, "DESTROY_HEAP") == 0) {
			deleteArrayOfLists(arrayOfListsFreeMemory);
			deleteArrayOfLists(arrayOfListsAllocatedMemory);
			break;

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

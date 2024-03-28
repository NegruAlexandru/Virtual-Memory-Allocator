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

	array_of_lists_t *arrayOfListsFreeMemory = NULL, *arrayOfListsAllocatedMemory = NULL;

	int nrOfMallocs = 0, nrOfFrees = 0, nrOfFragmentations = 0;

	long fragment_group = 0;

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

			arrayOfListsAllocatedMemory = (array_of_lists_t *) malloc(sizeof(array_of_lists_t));

			arrayOfListsAllocatedMemory->number = 0;

			arrayOfListsAllocatedMemory->memorySize = 0;

			arrayOfListsAllocatedMemory->lists = (doublyLinkedList_t **) malloc( 0);

		} else if (strcmp(choice, "MALLOC") == 0) {
			int mallocReturn = mallocFunction(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory, nrOfFragmentations, &fragment_group);

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
			char *data = (char *) malloc(600 * sizeof(char));
			int sizeToWrite = 0;

			scanf("%s", address);
			fgets(data, 600, stdin);

			char *token = strtok(data, "\"");
			token = strtok(NULL, "\"");

			sizeToWrite = atoi(strtok(NULL, " "));

			if (sizeToWrite > (int)strlen(token)) {
				sizeToWrite = strlen(token);
			}

			long addressLong = base16to10(removeHexaPrefix(address));

			if (isSpaceToWrite(arrayOfListsAllocatedMemory, addressLong, sizeToWrite)) {
				writeToAllocatedMemory(arrayOfListsAllocatedMemory, token, addressLong, sizeToWrite);
			} else {
				printf("Segmentation fault (core dumped)\n");
				printMemoryDump(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory, nrOfMallocs, nrOfFrees, nrOfFragmentations);\
				deleteArrayOfLists(arrayOfListsFreeMemory);
				deleteArrayOfLists(arrayOfListsAllocatedMemory);
				free(data);
				break;
			}

			free(data);

		} else if (strcmp(choice, "READ") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));
			int sizeToRead = 0;

			scanf("%s %d", address, &sizeToRead);

			long addressLong = base16to10(removeHexaPrefix(address));

			if (isRequestedMemoryAllocated(arrayOfListsAllocatedMemory, addressLong, sizeToRead)) {
				char *data = readFromAllocatedMemory(arrayOfListsAllocatedMemory, addressLong, sizeToRead);
				for (int i = 0; i < sizeToRead; i++) {
					printf("%c", data[i]);
				}

				printf("\n");

				free(data);
			} else {
				printf("Segmentation fault (core dumped)\n");
				printMemoryDump(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory, nrOfMallocs, nrOfFrees, nrOfFragmentations);
				deleteArrayOfLists(arrayOfListsFreeMemory);
				deleteArrayOfLists(arrayOfListsAllocatedMemory);
				break;
			}

		} else if (strcmp(choice, "DESTROY_HEAP") == 0) {
			deleteArrayOfLists(arrayOfListsFreeMemory);
			deleteArrayOfLists(arrayOfListsAllocatedMemory);
			break;

		} else if (strcmp(choice, "DUMP_MEMORY") == 0) {
			printMemoryDump(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory, nrOfMallocs, nrOfFrees, nrOfFragmentations);
		} else if (strcmp(choice, "info") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));
			scanf("%s", address);

			long addressLong = base16to10(removeHexaPrefix(address));

			node_t *block = getNodeByAddress(arrayOfListsFreeMemory, addressLong);

			printf("Block address: %lx, origin: %ld\n", block->address, block->origin);
		} else if (strcmp(choice, "origin") == 0) {
			int i = 0;
			scanf("%d", &i);

			for (int j = 0; j < arrayOfListsFreeMemory->number; j++) {
				doublyLinkedList_t *list = arrayOfListsFreeMemory->lists[j];
				node_t *current = list->head;

				while (current != NULL) {
					if (current->origin == i) {
						printf("Block address: %lx, origin: %ld\n", current->address, current->origin);
					}

					current = current->next;
				}
			}

		} else {
			printf("Invalid command\n");
		}
	}

	return 0;
}

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

		} else if (strcmp(choice, "DUMP_MEMORY") == 0) {
			if (arrayOfListsAllocatedMemory == NULL) {
				arrayOfListsAllocatedMemory = (arrayOfLists_t *) malloc(sizeof(arrayOfLists_t ));

				arrayOfListsAllocatedMemory->number = 0;

				arrayOfListsAllocatedMemory->memorySize = 0;



				printMemoryDump(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory);
			} else {
				printMemoryDump(arrayOfListsFreeMemory, arrayOfListsAllocatedMemory);
			}
		} else if (strcmp(choice, "exit") == 0) {
			break;
		} else {
			printf("Invalid command\n");
		}
	}

	return 0;
}

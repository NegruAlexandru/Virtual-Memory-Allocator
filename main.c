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

	doublyLinkedList_t **arrayOfLists = NULL;

	int number = 0, memory_size = 0;

	while (1) {
		char choice[MAX_INPUT_SIZE];
		scanf("%s", choice);

		if (strcmp(choice, "INIT_HEAP") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));

			scanf("%s %d %d", address, &number, &memory_size);

			int addressInt = base16to10(removeHexaPrefix(address));

			free(address);

			arrayOfLists = createArrayOfListsWithIncreasingSize(addressInt, number, memory_size);
		} else if (strcmp(choice, "DUMP_MEMORY") == 0) {
			printMemoryDump(arrayOfLists, number, memory_size);
		} else if (strcmp(choice, "exit") == 0) {
			break;
		} else {
			printf("Invalid command\n");
		}
	}

	return 0;
}
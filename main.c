//Negru Alexandru 314CAb 2023-2024

#include <stdio.h>
#include <string.h>
#include "lists_structs.h"
#include "command_functions.h"

int main(void)
{
	// create the variables
	aol_t *aol_free_memory = NULL, *aol_allocated_memory = NULL;

	int nr_mallocs = 0, nr_frees = 0, nr_fragmentations = 0, free_mode = 0;

	long fragment_group = 0;

	char choice[MAX_INPUT_SIZE];
	char command[MAX_INPUT_SIZE];

	// read the commands
	while (scanf("%s", choice)) {
		// store the input
		fgets(command, MAX_INPUT_SIZE, stdin);

		// check the command
		if (strcmp(choice, "INIT_HEAP") == 0) {
			command_init(command, &aol_free_memory, &aol_allocated_memory,
						 &free_mode);

		} else if (strcmp(choice, "MALLOC") == 0) {
			command_malloc(command, aol_free_memory, aol_allocated_memory,
						   &nr_mallocs, &nr_fragmentations, &fragment_group);

		} else if (strcmp(choice, "FREE") == 0) {
			command_free(command, aol_free_memory, aol_allocated_memory,
						 &nr_frees, free_mode);

		} else if (strcmp(choice, "WRITE") == 0) {
			command_write(command, aol_free_memory, aol_allocated_memory,
						  nr_mallocs, nr_frees, nr_fragmentations);

		} else if (strcmp(choice, "READ") == 0) {
			command_read(command, aol_free_memory, aol_allocated_memory,
						 nr_mallocs, nr_frees, nr_fragmentations);

		} else if (strcmp(choice, "DESTROY_HEAP") == 0) {
			command_destroy(aol_free_memory, aol_allocated_memory);

		} else if (strcmp(choice, "DUMP_MEMORY") == 0) {
			command_dump(aol_free_memory, aol_allocated_memory,
						 nr_mallocs, nr_frees, nr_fragmentations);
		}  else {
			// invalid command message
			printf("Invalid command\n");
		}
	}
}

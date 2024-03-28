#include "datatype_functions.h"
#include <stdlib.h>
#include "lists_functions.h"

long base16to10(char *hex) {
	long number = 0;
	int i = 0;

	while (hex[i] != '\0') {
		number *= 16;
		if (hex[i] >= '0' && hex[i] <= '9') {
			number += hex[i] - '0';
		} else if (hex[i] >= 'A' && hex[i] <= 'F') {
			number += hex[i] - 'A' + 10;
		} else if (hex[i] >= 'a' && hex[i] <= 'f') {
			number += hex[i] - 'a' + 10;
		}
		i++;
	}

	free(hex);
	return number;
}

char *remove_hexa_prefix(char *hex) {
	char *newHex = (char *) malloc(10 * sizeof(char));
	DIE(!newHex, "malloc");
	int i = 2;

	while (hex[i] != '\0') {
		newHex[i - 2] = hex[i];
		i++;
	}

	newHex[i - 2] = '\0';

	free(hex);
	return newHex;
}

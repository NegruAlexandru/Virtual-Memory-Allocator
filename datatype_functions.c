#include "datatype_functions.h"
#include <stdlib.h>

char *base10to16(int number) {
	char *hex = (char *) malloc(10 * sizeof(char));
	int i = 0;

	if (number == 0) {
		hex[i++] = '0';
		hex[i] = '\0';
		return hex;
	}

	while (number != 0) {
		int remainder = number % 16;
		char digit;
		if (remainder < 10) {
			digit = remainder + '0';
		} else {
			digit = remainder + 'A' - 10;
		}
		hex[i++] = digit;
		number /= 16;
	}
	hex[i] = '\0';

	int j = 0;
	for (j = 0; j < i / 2; j++) {
		char aux = hex[j];
		hex[j] = hex[i - j - 1];
		hex[i - j - 1] = aux;
	}

	return hex;
}

int base16to10(char *hex) {
	int number = 0;
	int i = 0;
	while (hex[i] != '\0') {
		number *= 16;
		if (hex[i] >= '0' && hex[i] <= '9') {
			number += hex[i] - '0';
		} else {
			number += hex[i] - 'A' + 10;
		}
		i++;
	}
	return number;
}

char *addHexaPrefix(char * hex) {
	char *newHex = (char *) malloc(10 * sizeof(char));
	newHex[0] = '0';
	newHex[1] = 'x';
	int i = 2;
	while (hex[i - 2] != '\0') {
		newHex[i] = hex[i - 2];
		i++;
	}
	newHex[i] = '\0';
	return newHex;
}

char *removeHexaPrefix(char * hex) {
	char *newHex = (char *) malloc(10 * sizeof(char));
	int i = 2;
	while (hex[i] != '\0') {
		newHex[i - 2] = hex[i];
		i++;
	}
	newHex[i - 2] = '\0';
	return newHex;
}
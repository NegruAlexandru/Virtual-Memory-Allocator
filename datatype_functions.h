#pragma once
#ifndef DATATYPE_FUNCTIONS_H
#define DATATYPE_FUNCTIONS_H
char *base10to16(int number);
int base16to10(char *hex);
char *addHexaPrefix(char * hex);
char *removeHexaPrefix(char * hex);
#endif

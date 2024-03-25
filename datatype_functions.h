#pragma once
#ifndef DATATYPE_FUNCTIONS_H
#define DATATYPE_FUNCTIONS_H

char *base10to16(long number);
long base16to10(char *hex);
char *addHexaPrefix(char * hex);
char *removeHexaPrefix(char * hex);
char *removeDoubleQuotes(char *string);

#endif

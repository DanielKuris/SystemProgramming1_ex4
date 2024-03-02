#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Reverses a string in place.
char *reverseStr(char *str);

// Converts an integer to a 4-bit binary string representation.
char *intToBinary(int num, char *binary);

// Converts a binary string to an integer.
int binaryToInt(char *binary);

// Inverts '0' to '1' and '1' to '0' in a binary string.
void encode(char *binary);

#endif /* MY_FUNCTIONS_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// Reversing a string 
char *reverseStr(char *str) {

    size_t len = strlen(str);

    // Initialize pointers for swapping
    char *start = str;
    char *end = str + len - 1;

    // Swap characters until start and end pointers meet
    while (start < end) {
        // Swap characters pointed to by start and end
        char temp = *start;
        *start = *end; //Make start's content the end's content
        *end = temp; //Make end's content the start's saved content

        // Move start pointer forward and end pointer backward
        start++;
        end--;
    }

    return str;
}

// Convert integer from [0,1,2,..,9] to binary string
char *intToBinary(int num, char *binary) {
    int count = 0;

    while (num > 0) {
        binary[count++] = (char)((num % 2) + '0'); // Convert binary digit to character '0' or '1'
        num /= 2;
    }

    // Pad the binary string with leading zeros to ensure it is 4 bits long
    while (count < 4) {
        binary[count++] = '0';
    }

    // Add null terminator at the end of the binary string
    binary[count] = '\0';

    // Reverse the binary string to obtain the correct binary representation
    reverseStr(binary);

    return binary;
}


// Converting binary to an integer
int binaryToInt(char *binary) {
    int result = 0;
    int len = strlen(binary);

    // Iterate over each binary digit in the string from right to left
    for (int i = len - 1; i >= 0; i--) {
        // Convert binary digit character to integer value
        int digit = binary[i] - '0';
        
        // Update the result based on the binary digit and its position
        result += digit * (int)pow(2, len - i - 1);
    }

    return result;
}


// Changing '0' to '1' and the opposite in a binary string
void encode(char *binary){
    int len = strlen(binary);

    for(int i=0; i < len; i++){

        if (binary[i] == '0')
            binary[i] = '1';

        else
            binary[i] = '0';
    }
}
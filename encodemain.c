#include <stdio.h>
#include <string.h>
#include "compress.h"

#define SIZE 4

int main(int argc, char *argv[]) {
    //Invalid command
    if (argc != 3) {
        printf("Usage: ./digitcompress -c file.txt\n");
        printf("       ./digitcompress -d file.bin\n");
        return 1;
    }

    char *option = argv[1];
    char *filename = argv[2];

    char *extension = strrchr(filename, '.'); // Find the last occurrence of '.' in the filename
    if (extension == NULL) {
        printf("Invalid filename: %s\n", filename);
        return 1;
    }
    extension++; // Move to the character after '.'

    if (strcmp(option, "-c") == 0) {
        // Compression operation
        if (strcmp(extension, "txt") == 0) {
            printf("Compressing text file: %s\n", filename);

            // Construct the new filename for the binary file
            char binFilename[strlen(filename) + 5]; // +5 for ".bin" and null terminator
            strcpy(binFilename, filename);
            strcat(binFilename, ".bin");

            // Clear the binary file
            FILE *binFile = fopen(binFilename, "wb");
            if (binFile == NULL) {
                printf("Error clearing binary file.\n");
                return 1;
            }

            fclose(binFile); // Close the binary file

            // Reopen the binary file for appending
            binFile = fopen(binFilename, "ab");
            if (binFile == NULL) {
                printf("Error opening binary file for appending.\n");
                return 1;
            }

            
            // Open the text file for reading
            FILE *txtFile = fopen(filename, "r");
            if (txtFile == NULL) {
                printf("Error opening text file for reading.\n");
                return 1;
            }

            int digitCount = 0; // Initialize count of digits

            // Read characters from the text file until EOF is reached
            int ch;

            //Allocating size of 5 chars (binary + null terminator)
            char *binary = (char *)malloc((SIZE+1)*sizeof(char));

            // Check if memory allocation was successful
            if (binary == NULL) {
                printf("Memory allocation failed for binary.\n");
                return 1;
            }

            while ((ch = fgetc(txtFile)) != EOF) {

                // Check if the character is a digit
                if (ch >= '0' && ch <= '9') {

                    // Convert the character to its corresponding integer value
                    int digit = ch - '0';
                    
                    //Converting digit to binary string
                    intToBinary(digit, binary);

                    //Encoding the binary string
                    encode(binary);
                    
                    //Add the encoded string to the file    
                    size_t items_written = fwrite(binary, sizeof(char), strlen(binary), binFile);

                    //Update amount of digits that were read so far
                    digitCount++;

                    //Check if the whole of binary was written in the bin file
                    if (items_written != strlen(binary)) {

                        printf("Error writing to binary file.\n");
                        //Free the binary string
                        free(binary);

                        // Close the text file
                        fclose(txtFile);

                        // Close the binary file
                        fclose(binFile);
                        return 1;
                    }
                }
                else{
                    printf("Invalid input.\n");
                    printf("Encountered character: %c\n", ch);
                    if(ch == ' ' || ch == '\n')
                        printf("Encountered space. \n");
                    //Free the binary string
                    free(binary);

                    // Close the text file
                    fclose(txtFile);

                    // Close the binary file
                    fclose(binFile);
                    return 1;
                }
            }

            // Check if the number of digits is odd
            if (digitCount % 2 != 0) {

                // Append four '0' characters (binary representation of 0)
                binary = intToBinary(0, binary);

                size_t items_written = fwrite(binary, sizeof(char), strlen(binary), binFile);
                //Check if the whole of binary was written in the bin file
                if (items_written != strlen(binary)) {

                    printf("Error writing to binary file.\n");
                    //Free the binary string
                    free(binary);

                    // Close the text file
                    fclose(txtFile);

                    // Close the binary file
                    fclose(binFile);
                    return 1;
                }
            }

            //Free the binary string
            free(binary);

            // Close the text file
            fclose(txtFile);

            // Close the binary file
            fclose(binFile);
           
        } else {
            //Invalid command
            printf("Invalid file type: %s\n", extension);
            printf("Usage: ./digitcompress -c file.txt\n");
            printf("       ./digitcompress -d file.bin\n");
            return 1;
        }
    } else if (strcmp(option, "-d") == 0) {
        // Decompression operation
        if (strcmp(extension, "bin") == 0) {
            printf("Decompressing binary file: %s\n", filename);

            // Construct the new filename for the text file
            char textFilename[strlen(filename) + 5]; // +5 for ".txt" and null terminator
            strcpy(textFilename, filename);
            strcat(textFilename, ".txt");

            // Clear the text file
            FILE *txtFile = fopen(textFilename, "w");
            if (txtFile == NULL) {
                printf("Error clearing text file.\n");
                return 1;
            }

            fclose(txtFile); // Close the text file

            // Reopen the text file for appending
            txtFile = fopen(textFilename, "a");
            if (txtFile == NULL) {
                printf("Error opening text file for appending.\n");
                return 1;
            }

            // Open the binary file for reading
            FILE *binFile = fopen(filename, "r");
            if (binFile == NULL) {
                printf("Error opening binary file for reading.\n");
                return 1;
            }

            // Read characters from the binary file until EOF is reached
            int ch;
            int count = 0; //Amount of binary digits in ' binary '

            //Allocating size of 5 chars (binary + null terminator)
            char *binary = (char *)malloc((SIZE+1)*sizeof(char));

            while ((ch = fgetc(binFile)) != EOF) {
                // Check if the character is '0' or '1'
                if (ch == '0' || ch == '1') {
                    // Add the character to the binary buffer
                    binary[count++] = (char)ch + '0';
                    // If the buffer is filled, process and write to the text file
                    if (count == SIZE) {
                        // Null-terminate the binary string
                        binary[SIZE] = '\0';
                        // Remove encoding
                        encode(binary);
                        // Convert binary string to decimal
                        int decimal = binaryToInt(binary);
                        // Write the decimal number to the text file
                        fprintf(txtFile, "%d", decimal);
                        // Reset the count for the next binary number
                        count = 0;
                    }
                } else {
                    printf("Invalid input.\n");
                    // Close the files and return with an error
                    fclose(txtFile);
                    fclose(binFile);
                    free(binary);
                    return 1;
                }
            }

            // If there are remaining digits in the buffer, process and write them
            if (count > 0) {
                // Null-terminate the binary string
                binary[count] = '\0';
                // Convert binary string to decimal
                int decimal = binaryToInt(binary);
                // Write the decimal number to the text file
                fprintf(txtFile, "%d", decimal);
            }

            // Close the files
            fclose(txtFile);
            fclose(binFile);
            free(binary);

        } else {
            //Invalid command
            printf("Invalid file type: %s\n", extension);
            printf("Usage: ./digitcompress -c file.txt\n");
            printf("       ./digitcompress -d file.bin\n");
            return 1;
        }
    } else {
        //Invalid command
        printf("Invalid file type: %s\n", extension);
        printf("Usage: ./digitcompress -c file.txt\n");
        printf("       ./digitcompress -d file.bin\n");
        return 1;
    }

    return 0;
}

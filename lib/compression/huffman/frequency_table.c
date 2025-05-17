#include <stdio.h>

/*******************************************************************************
 * Create frequency table.
 * The frequency table is an array that tracks the occurrences of each byte.
 *
 * inputs:
 * - input: name of the file to create frequency table from
 * - frequency_table: Tracks the number of occurences of each byte
 * outputs:
 * - none
 ******************************************************************************/
void create_frequency_table(const char *input, unsigned int frequency_table[256]) {

    /* Open the input file */
    FILE *input_file_pointer = fopen(input, "rb");
    if (!input_file_pointer) {
        printf("Error opening input file: %s\n", input);
        return;
    }

    /* Initialize each byte to occur 0 times initially */
    int i;
    for (i = 0; i < 256; i++) {
        frequency_table[i] = 0;
    }

    /* Read the input file - 1 byte at a time */
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, input_file_pointer) == 1) {

        /* Increment the occurence count of this byte */
        frequency_table[byte] += 1;
    }

    /* Close the input file */
    fclose(input_file_pointer);
}

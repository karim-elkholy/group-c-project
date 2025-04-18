#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

/*******************************************************************************
 * Converts an array of bytes to a string of hex characters.
 *
 * inputs:
 * - input - The input array of bytes
 * - output - The output string of hex characters
 * outputs:
 * - None.
 ******************************************************************************/
char *convert_bytes_to_hex_string(const unsigned char *input, int input_size)
{
    /* Allocate memory for the output string. */
    char *output = (char * ) malloc(input_size * 2 + 1);

    /* Convert the input bytes to a string of hex characters. */
    int i;
    for (i = 0; i < input_size; i++)
    {
        sprintf(output + (i * 2), "%02X", input[i]);
    }

    /* Null-terminate the string. */
    output[i * 2] = '\0';

    /* Return the output string. */
    return output;
}

/*******************************************************************************
 * Testing helper function
 * Convert a string of hex characters to an array of bytes.
 * 
 * Expects hex strings to be in the following format.
 *  VALID: 0001020304050
 *  INVALID: 0x000x10x20x30x40x50
 * 
 * inputs:
 * - input - The input string of hex characters. Must be even length.
 * - output - The output array of bytes
 * outputs:
 * - None.
 ******************************************************************************/
unsigned char *convert_hex_string_to_bytes(const char *input)
{
    /* Declare variables */
    int num_bytes;
    unsigned char *output;
    int i;  

    /* If the input string starts with 0x */
    if (input[0] == '0' && input[1] == 'x') {
        printf("input starts with 0x\n");
        printf("Valid format: 0001020304050\n");
        printf("Invalid format: 0x000x10x20x30x40x50\n");
        exit(0);
    }

    /* Determine the number of bytes in the input string. */
    num_bytes = strlen(input) / 2;

    /* Allocate memory for the output array of bytes. */
    output = (unsigned char *) malloc((num_bytes + 1) * sizeof(unsigned char));
    output[num_bytes] = '\0';

    /* Convert the input string of hex characters to an array of bytes. */
    for (i = 0; i < num_bytes; i++)
    {
        /* Convert the hex character to a byte. */
        char hex_byte[3];
        hex_byte[0] = input[i * 2];
        hex_byte[1] = input[i * 2 + 1];
        hex_byte[2] = '\0';
        output[i] = (unsigned char) strtol(hex_byte, NULL, 16);
    }


    /* Return the output array of bytes. */
    return output;
}

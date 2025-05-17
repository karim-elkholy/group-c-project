#include <stdio.h>
#include <string.h>

#include "compression/compression.h"
#include "compression/huffman/frequency_table.h"
#include "compression/huffman/tree.h"
#include "compression/huffman/codes.h"
#include "utils/bitops.h"

/*******************************************************************************
 * Writes the compressed data to the file.
 *
 * inputs:
 * - input_file: The file to read from
 * - output_file: The file to write compressed data to
 * - codes: The Huffman codes to use for compression
 * outputs:
 * - none
 ******************************************************************************/
void write_compressed_data(FILE *input_file, FILE *output_file, char *codes[256]) {

    /* Buffer for writing bits */
    /* Waits until 8 bits(1 byte) is collected before writing to file */
    unsigned char buffer = 0;

    /* Keeps track of the number of bits in the buffer */
    int bit_count = 0;

    /* Reserve 1 byte for the leftover bits count */
    unsigned char leftover_bits_count = 0;
    long leftover_bits_position = ftell(output_file);
    fwrite(&leftover_bits_count, sizeof(unsigned char), 1, output_file);

    /* Reserve unsigned int for the compressed data size */
    unsigned int compressed_data_size = 0;
    fwrite(&compressed_data_size, sizeof(unsigned int), 1, output_file);

    /* Loop until there are no more bytes to read */
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, input_file) == 1) {

        /* Update the compressed data size */
        compressed_data_size += 1;

        /* Get the Huffman code for the byte */
        char *code = codes[byte];

        /* Add the Huffman code to the buffer */
        int i;
        for (i = 0; i < strlen(code); i++) {
            /* Get the bit for the current depth in the code */
            unsigned char bit = code[i] == '1' ? 1 : 0;

            /* Add bit to buffer */
            buffer = append_bit(buffer, bit);

            /* Increment the bit count */
            bit_count += 1;

            /* If buffer is full, write it to file */
            if (bit_count == 8) {
                /* Write the full byte to the file */
                fwrite(&buffer, sizeof(unsigned char), 1, output_file);
                
                /* Reset the buffer */
                /* Binary form of 0 is 00000000 */
                buffer = 0;

                /* Reset the bit count */
                bit_count = 0;
            }
        }
    }

    /* Update the leftover bits count */
    leftover_bits_count = bit_count;

    /* If there are bits left in the buffer */
    if (bit_count > 0) {
        /* Calculate the bits not used in the buffer */
        int free_space = 8 - bit_count;

        /* Pad the buffer with 0 */
        /* Achieved by shifting the buffer left by the number of bits not used */
        buffer = shift_byte_left(buffer, free_space);

        /* Write the last buffer to the file */
        fwrite(&buffer, sizeof(unsigned char), 1, output_file);
    }

    /* Write the number of leftover bits & compressed data size */
    /* Needed so decompression knows how many bits to read */
    fseek(output_file, leftover_bits_position, SEEK_SET);
    fwrite(&leftover_bits_count, sizeof(unsigned char), 1, output_file);
    fwrite(&compressed_data_size, sizeof(unsigned int), 1, output_file);
}

/*******************************************************************************
 * Compresses a file using Huffman coding.
 *
 * inputs:
 * - uncompressed_file: The file to compress
 * - compressed_file: The file to write compressed data to
 * outputs:
 * - none
 ******************************************************************************/
void huffman_compress(const char *uncompressed_file, const char *compressed_file) {
    
    /* Create frequency table */
    unsigned int frequency_table[256];
    create_frequency_table(uncompressed_file, frequency_table);

    /* Create Huffman tree */
    HuffmanNode_t *huffman_tree = create_huffman_tree(frequency_table);

    /* Create a table of codes for each byte */
    char *codes[256] = { NULL };
    char temp_code_buffer[256];
    generate_huffman_codes(huffman_tree, temp_code_buffer, 0, codes);

    /* Open the compressed file */
    FILE *compressed_file_pointer = fopen(compressed_file, "wb");
    if (!compressed_file_pointer) {
        printf("Error opening output file: %s\n", compressed_file);
        return;
    }

    /* Write the frequency table to the compressed file */
    fwrite(frequency_table, sizeof(unsigned int), 256, compressed_file_pointer);

    /* Open the uncompressed file */
    FILE *uncompressed_file_pointer = fopen(uncompressed_file, "rb");
    if (!uncompressed_file_pointer) {
        printf("Error opening input file: %s\n", uncompressed_file);
        return;
    }

    /* Write the compressed data to the compressed file */
    write_compressed_data(uncompressed_file_pointer, compressed_file_pointer, codes);

    /* Close the file pointers & clean up memory */
    fclose(compressed_file_pointer);
    fclose(uncompressed_file_pointer);
    free_huffman_tree(huffman_tree);
}


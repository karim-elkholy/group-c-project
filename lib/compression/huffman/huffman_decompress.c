#include <stdio.h>

#include "compression/compression.h"
#include "compression/huffman/tree.h"
#include "utils/bitops.h"


/*******************************************************************************
 * Reads the compressed data from the file.
 *
 * inputs:
 * - input_file: The file to read compressed data from
 * - root: The root of the Huffman tree.
 * outputs:
 * - none
 ******************************************************************************/
void read_compressed_data(FILE *input_file, FILE *output_file, HuffmanNode_t *root) {

    /* Current node in the Huffman tree */
    HuffmanNode_t *current_node = root;
    
    /* Read the leftover bits count from the end of file */
    unsigned char leftover_bits;
    fread(&leftover_bits, sizeof(unsigned char), 1, input_file);

    /* Read the compressed data size */
    unsigned int compressed_data_size;
    fread(&compressed_data_size, sizeof(unsigned int), 1, input_file);

    /* Loop until there are no more bytes to read */
    int i;
    for (i = 0; i < compressed_data_size; i++) {

        /* Read the next byte */
        unsigned char byte;
        fread(&byte, sizeof(unsigned char), 1, input_file);

        /* For the last byte, only process the leftover bits */
        int bits_to_process = (i == compressed_data_size - 1) ? leftover_bits : 8;

        /* Process each bit in the byte */
        int j;
        for (j = 0; j < bits_to_process; j++) {

            /* Get the current bit (starting from most significant bit) */
            unsigned char bit = get_bit_msb(byte, j);

            /* If the bit is 0, go left otherwise go right */
            current_node = (bit == 0) ? current_node->left : current_node->right;

            /* If the current node is a leaf node */
            if (current_node->left == NULL && current_node->right == NULL) {
                /* Write the byte to the output file */
                fwrite(&current_node->data, sizeof(unsigned char), 1, output_file);

                /* Reset the current node to the root */
                current_node = root;
            }

        }
    }
}


/*******************************************************************************
 * Decompresses a file using Huffman coding.
 *
 * inputs:
 * - compressed_file: The file to decompress
 * - uncompressed_file: The file to write decompressed data to
 * outputs:
 * - none
 ******************************************************************************/
void huffman_decompress(const char *compressed_file, const char *uncompressed_file) {

    /* Open the compressed file */
    FILE *compressed_file_pointer = fopen(compressed_file, "rb");
    if (!compressed_file_pointer) {
        printf("Error opening input file: %s\n", compressed_file);
        return;
    }

    /* Read the frequency table from the compressed file */
    unsigned int frequency_table[256];
    fread(frequency_table, sizeof(unsigned int), 256, compressed_file_pointer);

    /* Create Huffman tree */
    HuffmanNode_t *huffman_tree = create_huffman_tree(frequency_table);

    /* Open the uncompressed file */
    FILE *uncompressed_file_pointer = fopen(uncompressed_file, "wb");
    if (!uncompressed_file_pointer) {
        printf("Error opening output file: %s\n", uncompressed_file);
        return;
    }    

    /* Read the compressed data from the compressed file */
    read_compressed_data(compressed_file_pointer, uncompressed_file_pointer, huffman_tree);
    
    /* Close the file pointers*/
    fclose(compressed_file_pointer);
    fclose(uncompressed_file_pointer);
    free_huffman_tree(huffman_tree);
}

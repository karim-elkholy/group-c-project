#include <stdio.h>
#include <string.h>

#include "utils/compression.h"

/*******************************************************************************
 * Compresses the content of a file using Run-Length Encoding (RLE).
 *
 * inputs:
 * - input: name of the file to compress
 * - output: name of the file to write compressed data to
 * outputs:
 * - none
 *******************************************************************************/
void compress_file(const char *input, const char *output) {
    FILE *fin = fopen(input, "rb");
    if (!fin) {
        printf("Error opening input file: %s\n", input);
        return;
    }

    FILE *fout = fopen(output, "wb");
    if (!fout) {
        printf("Error opening output file: %s\n", output);
        fclose(fin);
        return;
    }

    char prev;
    if (fread(&prev, sizeof(char), 1, fin) != 1) {
        fclose(fin);
        fclose(fout);
        return;
    }

    int count = 1;
    char curr;
    while (fread(&curr, sizeof(char), 1, fin) == 1) {
        if (curr == prev) {
            count++;
        } else {
            fwrite(&prev, sizeof(char), 1, fout);
            fwrite(&count, sizeof(int), 1, fout);
            prev = curr;
            count = 1;
        }
    }

    fwrite(&prev, sizeof(char), 1, fout);
    fwrite(&count, sizeof(int), 1, fout);

    fclose(fin);
    fclose(fout);
}

/*******************************************************************************
 * Decompresses a file previously compressed with RLE.
 *
 * inputs:
 * - input: name of the file to decompress
 * - output: name of the file to write decompressed data to
 * outputs:
 * - none
 *******************************************************************************/
void decompress_file(const char *input, const char *output) {
    FILE *fin = fopen(input, "rb");
    if (!fin) {
        printf("Error opening input file: %s\n", input);
        return;
    }

    FILE *fout = fopen(output, "wb");
    if (!fout) {
        printf("Error opening output file: %s\n", output);
        fclose(fin);
        return;
    }

    char ch;
    int count;
    int i = 0;
    /* Read each character and repeat count */
    while (fread(&ch, sizeof(char), 1, fin) == 1 &&
       fread(&count, sizeof(int), 1, fin) == 1) {
        for (i=0; i < count; i++) {
            fputc(ch, fout);
        }
    }

    fclose(fin);
    fclose(fout);
}

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
    FILE *fin = fopen(input, "rb");
    if (!fin) {
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
    while (fread(&byte, sizeof(unsigned char), 1, fin) == 1) {

        /* Increment the occurence count of this byte */
        frequency_table[byte] += 1;
    }

    /* Close the input file */
    fclose(fin);
}

/*******************************************************************************
 * Create Huffman tree.
 *
 * inputs:
 * - frequency_table: The frequency table to create the Huffman tree from
 * outputs:
 * - none
 ******************************************************************************/
void create_huffman_tree(unsigned int frequency_table[256]) {

}

/*******************************************************************************
 * Compresses a file using Huffman coding.
 *
 * inputs:
 * - uncompressed_file: The file to compress
 * - compressed_file: The file to write compressed data to
 * outputs:
 * - none
 *******************************************************************************/
void compressionHuffman(const char *uncompressed_file, const char *compressed_file) {

    /* Create frequency table */
    unsigned int frequency_table[256];
    create_frequency_table(uncompressed_file, frequency_table);

}
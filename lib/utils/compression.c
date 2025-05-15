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
 * Compresses a file using RLE.
 *
 * inputs:
 * - input: name of the file to compress
 * - output: name of the file to write compressed data to
 * outputs:
 * - none
 *******************************************************************************/
void compress_file(const char *input, const char *output) {
 
}


#include <stdio.h>
#include <string.h>
#include "compression.h"

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
    FILE *fin = fopen(input, "r");
    if (!fin) {
        printf("Error opening input file: %s\n", input);
        return;
    }

    FILE *fout = fopen(output, "w");
    if (!fout) {
        printf("Error opening output file: %s\n", output);
        fclose(fin);
        return;
    }

    // Read the first character to begin compression
    char prev = fgetc(fin);
    if (prev == EOF) {
        fclose(fin);
        fclose(fout);
        return;
    }

    int count = 1;
    char curr;
    // Loop through remaining characters
    while ((curr = fgetc(fin)) != EOF) {
        if (curr == prev) {
            count++;
        } else {
            // Write the character and its repeat count
            fprintf(fout, "%c%d", prev, count);
            prev = curr;
            count = 1;
        }
    }
    // Write the last character and count
    fprintf(fout, "%c%d", prev, count);

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
    FILE *fin = fopen(input, "r");
    if (!fin) {
        printf("Error opening input file: %s\n", input);
        return;
    }

    FILE *fout = fopen(output, "w");
    if (!fout) {
        printf("Error opening output file: %s\n", output);
        fclose(fin);
        return;
    }

    char ch;
    int count;
    // Read each character and repeat count
    while (fscanf(fin, "%c%d", &ch, &count) == 2) {
        for (int i = 0; i < count; i++) {
            fputc(ch, fout);
        }
    }

    fclose(fin);
    fclose(fout);
}
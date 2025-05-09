#include <stdio.h>
#include "utils/compression.h"

int main() {
    const char *input = "test_input.txt";
    const char *compressed = "compressed.rle";
    const char *decompressed = "decompressed.txt";

    FILE *f = fopen(input, "wb");
    if (f == NULL) {
        printf("Failed to create test input.\n");
        return 1;
    }
    fprintf(f, "aaabbbcccaaa");
    fclose(f);

    compress_file(input, compressed);
    decompress_file(compressed, decompressed);

    printf("Compression and decompression completed.\n");

    FILE *fin1 = fopen(input, "rb");
    FILE *fin2 = fopen(decompressed, "rb");

    int match = 1;
    while (1) {
        int c1 = fgetc(fin1);
        int c2 = fgetc(fin2);
        if (c1 != c2) {
            match = 0;
            break;
        }
        if (c1 == EOF || c2 == EOF) break;
    }

    fclose(fin1);
    fclose(fin2);

    if (match)
        printf("Files match!\n");
    else
        printf("Files do NOT match!\n");

    return 0;
}
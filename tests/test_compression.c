#include <stdio.h>

#include "utils/compression.h"
#include "test_shared.h"

void test_compression() {

    const char *input = "test_input.txt";
    const char *content = "Hello World!";

    /* Write a file with content to test compression on */
    FILE *input_pointer = fopen(input, "wb");
    if (input_pointer == NULL) {
        printf("Failed to create test input.\n");
        return;
    }
    fwrite(content, sizeof(char), strlen(content), input_pointer);
    fclose(input_pointer);

    /* Compressed & decompress the content */
    const char *compressed = "compressed.huffman";
    compression_huffman(input, compressed);
    const char *decompressed = "decompressed.txt";
    decompress_huffman(compressed, decompressed);

    /* Check if the decompressed file matches the original */
    FILE *decompressed_pointer = fopen(decompressed, "rb");
    if (decompressed_pointer == NULL) {
        printf("Failed to open decompressed file.\n");
        return;
    }

    /* Read the decompressed file */
    char buffer[256];
    fread(buffer, sizeof(char), 256, decompressed_pointer);
    fclose(decompressed_pointer);

    /* Print a comparison of each byte */
    int i;
    for (i = 0; i < strlen(content); i++) {
        printf("Original %X(%c): Decompressed: %x(%c)\n", content[i], content[i], buffer[i], buffer[i]);
    }

    /* Compare using memcmp */
    if (memcmp(buffer, content, strlen(content)) == 0) {
        printf("Compression test passed!\n");
    } else {
        printf("Compression test failed!\n");
    }
}

int main() {
    test_run_method("Huffman compression", test_compression);
    return 0;

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
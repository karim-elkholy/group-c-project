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
}
#include <stdio.h>
#include "../lib/application/compression.c"

int main() {
    const char *input = "test_input.txt";
    const char *compressed = "compressed.txt";
    const char *decompressed = "decompressed.txt";

    FILE *f = fopen(input, "w");
    if (f == NULL) {
        printf("Failed to create test input.\n");
        return 1;
    }
    char a = 0x61; // 'a'
    
    fprintf(f, "aaabbbcccaaa");
    fclose(f);

    compress_file(input, compressed);
    decompress_file(compressed, decompressed);

    printf("Compression and decompression completed.\n");
    return 0;
}
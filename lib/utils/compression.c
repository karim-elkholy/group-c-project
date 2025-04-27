#include <stdio.h>
#include <string.h>
#include "compression.h"

void compress_file(const char *input, const char *output) {
    FILE *fin = fopen(input, "r");
    FILE *fout = fopen(output, "w");

    if (!fin || !fout) {
        printf("Error opening file!\n");
        return;
    }

    char prev = fgetc(fin);
    if (prev == EOF) {
        fclose(fin);
        fclose(fout);
        return;
    }

    int count = 1;
    char curr;
    while ((curr = fgetc(fin)) != EOF) {
        if (curr == prev) {
            count++;
        } else {
            fprintf(fout, "%c%d", prev, count);
            prev = curr;
            count = 1;
        }
    }
    fprintf(fout, "%c%d", prev, count);

    fclose(fin);
    fclose(fout);
}


void decompress_file(const char *input, const char *output) {
    FILE *fin = fopen(input, "r");
    FILE *fout = fopen(output, "w");

    if (!fin || !fout) {
        printf("Error opening file!\n");
        return;
    }

    char ch;
    int count;
    while (fscanf(fin, "%c%d", &ch, &count) == 2) {
        for (int i = 0; i < count; i++) {
            fputc(ch, fout);
        }
    }

    fclose(fin);
    fclose(fout);
}
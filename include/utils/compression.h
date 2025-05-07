#ifndef COMPRESSION_H
#define COMPRESSION_H

// Compresses a file using Run-Length Encoding (RLE).
// Parameters:
// - input:  path to the input file to compress
// - output: path to the output compressed file
void compress_file(const char *input, const char *output);

// Decompresses a file that was compressed using RLE.
// Parameters:
// - input:  path to the compressed input file
// - output: path to the output decompressed file
void decompress_file(const char *input, const char *output);

#endif
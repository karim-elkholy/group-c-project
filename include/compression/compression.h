#ifndef COMPRESSION_COMPRESSION_H
#define COMPRESSION_COMPRESSION_H

/*******************************************************************************
 * Compresses a file using Huffman coding.
 *
 * inputs:
 * - uncompressed_file: The file to compress
 * - compressed_file: The file to write compressed data to
 * outputs:
 * - none
 ******************************************************************************/
void huffman_compress(const char *uncompressed_file, const char *compressed_file);


/*******************************************************************************
 * Decompresses a file using Huffman coding.
 *
 * inputs:
 * - compressed_file: The file to decompress
 * - uncompressed_file: The file to write decompressed data to
 * outputs:
 * - none
 ******************************************************************************/
void huffman_decompress(const char *compressed_file, const char *uncompressed_file);



#endif


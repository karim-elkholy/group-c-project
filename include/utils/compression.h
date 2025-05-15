#ifndef COMPRESSION_H
#define COMPRESSION_H

/* Node for a Huffman tree */
/* Can represent a leaf or an internal node */
struct HuffmanNode {

    /* Data stored in the node */
    unsigned char data;

    /* Left child of the node */
    struct HuffmanNode *left;

    /* Right child of the node */
    struct HuffmanNode *right;

    /* Frequency of the node */
    /* For leaf nodes: Number of times the byte occurs in the file */
    /* For internal nodes: Sum of children's frequencies */
    unsigned int frequency;

};

typedef struct HuffmanNode HuffmanNode_t;

/*******************************************************************************
 * Compresses a file using Huffman coding.
 *
 * inputs:
 * - uncompressed_file: The file to compress
 * - compressed_file: The file to write compressed data to
 * outputs:
 * - none
 ******************************************************************************/
void compression_huffman(const char *uncompressed_file, const char *compressed_file);


/*******************************************************************************
 * Decompresses a file that was compressed using Huffman coding.
 *
 * inputs:
 * - compressed_file: The file to decompress
 * - uncompressed_file: The file to write decompressed data to
 ******************************************************************************/
void decompress_huffman(const char *compressed_file, const char *uncompressed_file);
#endif
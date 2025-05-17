#ifndef COMPRESSION_HUFFMAN_TREE_H
#define COMPRESSION_HUFFMAN_TREE_H

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
 * Create Huffman tree.
 *
 * inputs:
 * - frequency_table: The frequency table to create the Huffman tree from
 * outputs:
 * - The root of the Huffman tree
 ******************************************************************************/
HuffmanNode_t *create_huffman_tree(unsigned int frequency_table[256]);

/*******************************************************************************
 * Free the Huffman tree.
 *
 * inputs:
 * - node: The node to free
 * outputs:
 * - none
 ******************************************************************************/
void free_huffman_tree(HuffmanNode_t *node);

#endif


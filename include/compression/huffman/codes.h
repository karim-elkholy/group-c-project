#ifndef COMPRESSION_HUFFMAN_CODES_H
#define COMPRESSION_HUFFMAN_CODES_H

/*******************************************************************************
 * Generate the Huffman codes.
 * Codes are paths used to reach a specific byte in the tree.
 *
 * inputs:
 * - node: The node to generate the codes for
 * - code: The code to store the codes
 * - depth: The depth of the node
 * - codes: The codes table to store the codes. 256 for each possible byte.
 * outputs:
 * - none
 ******************************************************************************/
void generate_huffman_codes(HuffmanNode_t *node, char *code, int depth, char *codes[256]);

#endif


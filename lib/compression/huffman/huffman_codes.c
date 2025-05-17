#include <stdlib.h>
#include <string.h>

#include "compression/huffman/tree.h"

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
void generate_huffman_codes(HuffmanNode_t *node, char *code, int depth, char *codes[256]) {

    /* Do nothing if the node is NULL */
    if (node == NULL) {
        return;
    }

    /* If the node is a leaf node */
    if (node->left == NULL && node->right == NULL) {
        /* Null terminate the code */
        code[depth] = '\0';

        /* Copy the path to the byte in the codes table */
        codes[node->data] = (char *) malloc((strlen(code) + 1) * sizeof(char)); 
        strcpy(codes[node->data], code);
    
    /* If the node is an internal node */
    } else {

        /* Go left */
        /* 0 is added to the path to indicate the left direction */
        code[depth] = '0';
        generate_huffman_codes(node->left, code, depth + 1, codes);

        /* Go right */
        /* 1 is added to the path to indicate the right direction */
        code[depth] = '1';
        generate_huffman_codes(node->right, code, depth + 1, codes);
    }
}


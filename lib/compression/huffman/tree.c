#include <stdlib.h>

#include "compression/huffman/tree.h"

/*******************************************************************************
 * Sort the nodes by frequency in ascending order.
 *
 * inputs:
 * - nodes: The nodes to sort
 * - node_count: The number of nodes in the array
 * outputs:
 * - none
 ******************************************************************************/
void sort_nodes(HuffmanNode_t *nodes[], int node_count) {

    while ( 1 ) {

        /* Assume the nodes are sorted unless proven otherwise */
        int sorted = 1;

        /* Check if the nodes are sorted */
        int i;
        for (i = 1; i < node_count; i++) {

            /* If this node is lower than the previous node */
            if (nodes[i]->frequency < nodes[i - 1]->frequency) {
                
                /* Nodes are not sorted correctly */
                sorted = 0;

                /* Swap the nodes */
                HuffmanNode_t *temp = nodes[i];
                nodes[i] = nodes[i - 1];
                nodes[i - 1] = temp;

                /* Restart the while loop */
                break;
            }
        }

        /* Exit once nodes are sorted */
        if (sorted) {
            break;
        }
    }
}


/*******************************************************************************
 * Create Huffman tree.
 *
 * inputs:
 * - frequency_table: The frequency table to create the Huffman tree from
 * outputs:
 * - The root of the Huffman tree
 ******************************************************************************/
HuffmanNode_t *create_huffman_tree(unsigned int frequency_table[256]) {

    /* Store nodes */
    HuffmanNode_t *nodes[256] = {NULL};

    /* Track the number of nodes */
    int node_count = 0;

    /* Create leaf nodes for each byte that occurs in the file */
    int i;
    for (i = 0; i < 256; i++) {

        /* If the byte occurs at least once */
        if (frequency_table[i] > 0) {

            /* Create a leaf node for this byte */
            HuffmanNode_t *node = (HuffmanNode_t *) malloc(sizeof(HuffmanNode_t));
            node->data = i;
            node->frequency = frequency_table[i];

            /* Leaf nodes have no children */
            node->left = NULL;
            node->right = NULL;

            /* Add the node to the nodes array */
            nodes[node_count] = node;

            /* Increment the node count */
            node_count += 1;
        }
    }


    /* If there is more than 1 unique byte in the file */
    if ( node_count > 1 ) {

        /* Loop until only 1 node remains */
        while (node_count > 1) {

            /* Ensure the nodes are sorted by frequency */
            sort_nodes(nodes, node_count);

            /* Take the two nodes with the lowest frequency */
            HuffmanNode_t *node1 = nodes[0];
            HuffmanNode_t *node2 = nodes[1];

            /* Create an internal node */
            HuffmanNode_t *internal_node = (HuffmanNode_t *) malloc(sizeof(HuffmanNode_t));
            internal_node->frequency = node1->frequency + node2->frequency;
            internal_node->left = node1;
            internal_node->right = node2;

            /* Move every node in the array up by 2 positions */
            /* This effeectively removes the first two nodes from the array */
            for (i = 0; i < node_count - 2; i++) {
                nodes[i] = nodes[i + 2];
            }
            node_count -= 2;

            /* Add the internal node to the array */
            nodes[node_count] = internal_node;
            node_count += 1;
        }
    }

    /* Return the root of the tree */
    return nodes[0];
}

/*******************************************************************************
 * Free the Huffman tree.
 *
 * inputs:
 * - node: The node to free
 * outputs:
 * - none
 ******************************************************************************/
void free_huffman_tree(HuffmanNode_t *node) {

    /* If the node is not null */
    if (node != NULL) {

        /* If the node is a leaf node */
        if (node->left == NULL && node->right == NULL) {
            /* Free the node */
            free(node);
        
        /* If the node is an internal node */
        } else {
            /* Free the left child */
            free_huffman_tree(node->left);

            /* Free the right child */
            free_huffman_tree(node->right);

            /* Free the node */
            free(node);
        }
    }
}

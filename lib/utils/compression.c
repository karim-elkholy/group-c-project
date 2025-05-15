#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils/compression.h"
#include "utils/bitops.h"

/*******************************************************************************
 * Compresses the content of a file using Run-Length Encoding (RLE).
 *
 * inputs:
 * - input: name of the file to compress
 * - output: name of the file to write compressed data to
 * outputs:
 * - none
 *******************************************************************************/
void compress_file(const char *input, const char *output) {
    FILE *fin = fopen(input, "rb");
    if (!fin) {
        printf("Error opening input file: %s\n", input);
        return;
    }

    FILE *fout = fopen(output, "wb");
    if (!fout) {
        printf("Error opening output file: %s\n", output);
        fclose(fin);
        return;
    }

    char prev;
    if (fread(&prev, sizeof(char), 1, fin) != 1) {
        fclose(fin);
        fclose(fout);
        return;
    }

    int count = 1;
    char curr;
    while (fread(&curr, sizeof(char), 1, fin) == 1) {
        if (curr == prev) {
            count++;
        } else {
            fwrite(&prev, sizeof(char), 1, fout);
            fwrite(&count, sizeof(int), 1, fout);
            prev = curr;
            count = 1;
        }
    }

    fwrite(&prev, sizeof(char), 1, fout);
    fwrite(&count, sizeof(int), 1, fout);

    fclose(fin);
    fclose(fout);
}

/*******************************************************************************
 * Decompresses a file previously compressed with RLE.
 *
 * inputs:
 * - input: name of the file to decompress
 * - output: name of the file to write decompressed data to
 * outputs:
 * - none
 *******************************************************************************/
void decompress_file(const char *input, const char *output) {
    FILE *fin = fopen(input, "rb");
    if (!fin) {
        printf("Error opening input file: %s\n", input);
        return;
    }

    FILE *fout = fopen(output, "wb");
    if (!fout) {
        printf("Error opening output file: %s\n", output);
        fclose(fin);
        return;
    }

    char ch;
    int count;
    int i = 0;
    /* Read each character and repeat count */
    while (fread(&ch, sizeof(char), 1, fin) == 1 &&
       fread(&count, sizeof(int), 1, fin) == 1) {
        for (i=0; i < count; i++) {
            fputc(ch, fout);
        }
    }

    fclose(fin);
    fclose(fout);
}



/*
Huffman tree has two type of nodes.
1. Leaf nodes. 
    - Data is the byte. 
    - Frequency is the number of times the byte occurs in the file.
    - Have no children so left and right pointers are NULL.
2. Internal nodes.
    - Don't use data. It is NULL.
    - Frequency is the sum of children's frequencies. 
    - Have at least 1 child(left/right or both).

Step 1. Create a frequency table of the input file.
Step 2. Create a Huffman tree from the frequency table.
    - Root will be a leaf node if only 1 unique byte in the file.
    - Root will be an internal node if there is more than 1 unique byte in the file.

Step 3. Create Huffman codes for each byte in the tree.
    A huffman code consists of 2 parts:
        1. Path of the byte in the tree.
        2. How many bits used in the path. The deeper the byte in the tree, the longer the path.

    Each time we go left, 0 is added to the rightmost side of the path.
    Each time we go right, 1 is added to the rightmost side of the path.

Step 4. Write the uncompressed file to the compressed file (in terms of Huffman codes)
Because the compressed file is in terms of Huffman codes(aka bits) instead of bytes, 
This is where the saving comes from.
*/

/*******************************************************************************
 * Create frequency table.
 * The frequency table is an array that tracks the occurrences of each byte.
 *
 * inputs:
 * - input: name of the file to create frequency table from
 * - frequency_table: Tracks the number of occurences of each byte
 * outputs:
 * - none
 ******************************************************************************/
void create_frequency_table(const char *input, unsigned int frequency_table[256]) {

    /* Open the input file */
    FILE *input_file_pointer = fopen(input, "rb");
    if (!input_file_pointer) {
        printf("Error opening input file: %s\n", input);
        return;
    }

    /* Initialize each byte to occur 0 times initially */
    int i;
    for (i = 0; i < 256; i++) {
        frequency_table[i] = 0;
    }

    /* Read the input file - 1 byte at a time */
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, input_file_pointer) == 1) {

        /* Increment the occurence count of this byte */
        frequency_table[byte] += 1;
    }

    /* Close the input file */
    fclose(input_file_pointer);
}

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

void print_huffman_tree(HuffmanNode_t *node, int depth) {
    if (node == NULL) return;

    /* Indentation for depth */
    int i;
    for (i = 0; i < depth; i++) {
        printf("    "); /* 4 spaces per level */
    }

    /* Print the current node */
    if (node->left == NULL && node->right == NULL) {
        /* Leaf node */
        if (node->data >= 32 && node->data <= 126)
            printf("Leaf: '%c' (freq: %d)\n", node->data, node->frequency);
        else
            printf("Leaf: 0x%02X (freq: %d)\n", node->data, node->frequency);
    } else {
        /* Internal node */
        printf("%d.Internal (freq: %d)\n", depth + 1, node->frequency);
    }

    /* Traverse children */
    print_huffman_tree(node->left, depth + 1);
    print_huffman_tree(node->right, depth + 1);
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

/*******************************************************************************
* Reads the tree from the compressed file.
*
* inputs:
* - compressed_file: The file to read compressed data from
* - node: The Huffman node to read from the compressed file
* outputs:
* - none
*******************************************************************************/
void read_tree_from_file(FILE *compressed_file, HuffmanNode_t *node) {

    /* Read the marker to determine whether the node is a leaf node or an internal node */
    int marker;
    fread(&marker, sizeof(int), 1, compressed_file);

    /* If the node is a leaf node */
    if (marker == 1) {
        /* Read the byte & frequency */
        fread(&node->data, sizeof(unsigned char), 1, compressed_file);
        fread(&node->frequency, sizeof(unsigned int), 1, compressed_file);

        /* Leaf nodes have no children */
        node->left = NULL;
        node->right = NULL;

    /* If the node is an internal node */
    } else {

        /* Read the left child */
        node->left = (HuffmanNode_t *) malloc(sizeof(HuffmanNode_t));
        read_tree_from_file(compressed_file, node->left);

        /* Read the right child */
        node->right = (HuffmanNode_t *) malloc(sizeof(HuffmanNode_t));
        read_tree_from_file(compressed_file, node->right);

        /* Calculate the frequency */
        node->frequency = node->left->frequency + node->right->frequency;
    }
}

/*******************************************************************************
* Writes the tree to the compressed file.
*
* inputs:
* - compressed_file: The file to write compressed data to
* - node: The Huffman node to write to the compressed file
* outputs:
* - none
*******************************************************************************/
void write_tree_to_file(FILE *compressed_file, HuffmanNode_t *node) {

    /* Do nothing if the node is NULL */
    if (node == NULL) {
        return;
    }
    
    /* Marks whether the node is a leaf node or an internal node */
    int marker;

    /* If the node is a leaf node */
    if (node->left == NULL && node->right == NULL) {
        /* Mark leaf nodes with 1 */
        marker = 1;
        fwrite(&marker, sizeof(int), 1, compressed_file);

        /* Write the byte & frequency to the compressed file */
        fwrite(&node->data, sizeof(unsigned char), 1, compressed_file);
        fwrite(&node->frequency, sizeof(unsigned int), 1, compressed_file);

    /* If the node is an internal node */
    } else {
        /* Mark internal node with 0 */
        marker = 0;
        fwrite(&marker, sizeof(int), 1, compressed_file);

        /* Write the left child to the compressed file */
        write_tree_to_file(compressed_file, node->left);

        /* Write the right child to the compressed file */
        write_tree_to_file(compressed_file, node->right);
    }
}


/*******************************************************************************
 * Writes the compressed data to the file.
 *
 * inputs:
 * - input_file: The file to read from
 * - output_file: The file to write compressed data to
 * - codes: The Huffman codes to use for compression
 * outputs:
 * - none
 ******************************************************************************/
void write_compressed_data(FILE *input_file, FILE *output_file, HuffmanCode_t *codes) {


    /* Buffer for writing bits */
    /* Waits until 8 bits(1 byte) is collected before writing to file */
    unsigned char buffer = 0;

    /* Keeps track of the number of bits in the buffer */
    int bit_count = 0;

    /* Loop until there are no more bytes to read */
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, input_file) == 1) {

        /* Get the Huffman code for the byte */
        HuffmanCode_t code = codes[byte];
        
        /* Add the Huffman code to the buffer */
        int i;
        for (i = 0; i < code.path_length; i++) {

            /* Use the bits as the path in MSB-order */
            unsigned char bit = get_bit_msb(code.path, i);

            /* Add bit to buffer */
            buffer = append_bit(buffer, bit);

            /* Increment the bit count */
            bit_count += 1;

            /* When buffer is full, write it to file */
            if (bit_count == 8) {

                /* Write the full byte to the file */
                fwrite(&buffer, sizeof(unsigned char), 1, output_file);
                
                /* Reset the buffer */
                /* Binary form of 0 is 00000000 */
                buffer = 0;

                /* Reset the bit count */
                bit_count = 0;
            }
        }
    }

    /* Write any remaining bits in the buffer */
    if (bit_count > 0) {

        /* Calculate the bits not used in the buffer */
        int free_space = 8 - bit_count;

        /* Pad the buffer with 0 */
        int i;
        for ( i = 0; i < free_space; i++ ) {
            /* Append 0 */
            buffer = append_bit(buffer, 0);
        }
        
        /* Write the last buffer to the file */
        fwrite(&buffer, sizeof(unsigned char), 1, output_file);
    }

    /* Write the number of leftover bits */
    /* Needed so decompression knows how many bits to read */
    fwrite(&bit_count, sizeof(int), 1, output_file);
}


void write_compressed_data_new(FILE *input_file, FILE *output_file, char *codes[256]) {

    /* Buffer for writing bits */
    /* Waits until 8 bits(1 byte) is collected before writing to file */
    unsigned char buffer = 0;

    /* Keeps track of the number of bits in the buffer */
    int bit_count = 0;

    /* Reserve 1 byte for the leftover bits count */
    unsigned char leftover_bits_count = 0;
    long leftover_bits_position = ftell(output_file);
    fwrite(&leftover_bits_count, sizeof(unsigned char), 1, output_file);

    /* Reserve unsigned int for the compressed data size */
    unsigned int compressed_data_size = 0;
    fwrite(&compressed_data_size, sizeof(unsigned int), 1, output_file);

    /* Loop until there are no more bytes to read */
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, input_file) == 1) {

        /* Update the compressed data size */
        compressed_data_size += 1;

        /* Get the Huffman code for the byte */
        char *code = codes[byte];

        /* Add the Huffman code to the buffer */
        int i;
        for (i = 0; i < strlen(code); i++) {
            /* Get the bit for the current depth in the code */
            unsigned char bit = code[i] == '1' ? 1 : 0;

            /* Add bit to buffer */
            buffer = append_bit(buffer, bit);

            /* Increment the bit count */
            bit_count += 1;

            /* If buffer is full, write it to file */
            if (bit_count == 8) {
                /* Write the full byte to the file */
                fwrite(&buffer, sizeof(unsigned char), 1, output_file);
                
                /* Reset the buffer */
                /* Binary form of 0 is 00000000 */
                buffer = 0;

                /* Reset the bit count */
                bit_count = 0;
            }
        }
    }

    /* Update the leftover bits count */
    leftover_bits_count = bit_count;

    /* If there are bits left in the buffer */
    if (bit_count > 0) {
        /* Calculate the bits not used in the buffer */
        int free_space = 8 - bit_count;

        /* Pad the buffer with 0 */
        /* Achieved by shifting the buffer left by the number of bits not used */
        buffer = shift_byte_left(buffer, free_space);

        /* Write the last buffer to the file */
        fwrite(&buffer, sizeof(unsigned char), 1, output_file);
    }

    /* Write the number of leftover bits & compressed data size */
    /* Needed so decompression knows how many bits to read */
    fseek(output_file, leftover_bits_position, SEEK_SET);
    fwrite(&leftover_bits_count, sizeof(unsigned char), 1, output_file);
    fwrite(&compressed_data_size, sizeof(unsigned int), 1, output_file);

}

/*******************************************************************************
 * Reads the compressed data from the file.
 *
 * inputs:
 * - input_file: The file to read compressed data from
 * - root: The root of the Huffman tree.
 * outputs:
 * - none
 */
void read_compressed_data(FILE *input_file, FILE *output_file, HuffmanNode_t *root) {

    /* If root is null */
    if (root == NULL) {
        printf("Error: Huffman tree root is null\n");
        return;
    }

    /* Current node in the Huffman tree */
    HuffmanNode_t *current_node = root;

    /* Stores the position for the start of the compressed data */
    int start_position = ftell(input_file);

    /* Move to end of file to determine file size */
    fseek(input_file, 0, SEEK_END);
    int file_size = ftell(input_file);
    
    /* Read the leftover bits count from the end of file */
    int leftover_bits;
    fseek(input_file, file_size - sizeof(int), SEEK_SET);
    fread(&leftover_bits, sizeof(int), 1, input_file);

    /* Calculate compressed data size excluding leftover bits int at end */
    int compressed_data_size = file_size - start_position - sizeof(int);

    /* Move back to start of compressed data */
    fseek(input_file, start_position, SEEK_SET);

    /* Read the compressed data */
    int i;
    for (i = 0; i < compressed_data_size; i++) {

        /* Read the next byte */
        unsigned char byte;
        fread(&byte, sizeof(unsigned char), 1, input_file);
        
        /* For the last byte, only process the leftover bits */
        int bits_to_process = (i == compressed_data_size - 1) ? leftover_bits : 8;
        
        /* Process each bit in the byte */
        int j;
        for (j = 0; j < bits_to_process; j++) {

            /* Get the current bit (starting from leftmost bit) */
            unsigned char bit = get_bit_msb(byte, j);

            /* If the bit is 0, go left otherwise go right */
            current_node = (bit == 0) ? current_node->left : current_node->right;

            /* If the current node is a leaf node */
            if (current_node->left == NULL && current_node->right == NULL) {
                /* Write the byte to the output file */
                fwrite(&current_node->data, sizeof(unsigned char), 1, output_file);

                /* Reset the current node to the root */
                current_node = root;
            }
        }
    }
}

void read_compressed_data_new(FILE *input_file, FILE *output_file, HuffmanNode_t *root) {

    /* Current node in the Huffman tree */
    HuffmanNode_t *current_node = root;
    
    /* Read the leftover bits count from the end of file */
    unsigned char leftover_bits;
    fread(&leftover_bits, sizeof(unsigned char), 1, input_file);

    /* Read the compressed data size */
    unsigned int compressed_data_size;
    fread(&compressed_data_size, sizeof(unsigned int), 1, input_file);

    /* Loop until there are no more bytes to read */
    int i;
    for (i = 0; i < compressed_data_size; i++) {

        /* Read the next byte */
        unsigned char byte;
        fread(&byte, sizeof(unsigned char), 1, input_file);

        /* For the last byte, only process the leftover bits */
        int bits_to_process = (i == compressed_data_size - 1) ? leftover_bits : 8;

        /* Process each bit in the byte */
        int j;
        for (j = 0; j < bits_to_process; j++) {

            /* Get the current bit (starting from most significant bit) */
            unsigned char bit = get_bit_msb(byte, j);

            /* If the bit is 0, go left otherwise go right */
            current_node = (bit == 0) ? current_node->left : current_node->right;

            /* If the current node is a leaf node */
            if (current_node->left == NULL && current_node->right == NULL) {
                /* Write the byte to the output file */
                fwrite(&current_node->data, sizeof(unsigned char), 1, output_file);

                /* Reset the current node to the root */
                current_node = root;
            }

        }
    }
}


/*******************************************************************************
 * Compresses a file using Huffman coding.
 *
 * inputs:
 * - uncompressed_file: The file to compress
 * - compressed_file: The file to write compressed data to
 * outputs:
 * - none
 ******************************************************************************/
void compression_huffman(const char *uncompressed_file, const char *compressed_file) {
    
    /* Create frequency table */
    unsigned int frequency_table[256];
    create_frequency_table(uncompressed_file, frequency_table);

    /* Create Huffman tree */
    HuffmanNode_t *huffman_tree = create_huffman_tree(frequency_table);

    /* Create a table of codes for each byte */
    char *codes[256] = { NULL };
    char temp_code_buffer[256];
    generate_huffman_codes(huffman_tree, temp_code_buffer, 0, codes);

    /* Open the compressed file */
    FILE *compressed_file_pointer = fopen(compressed_file, "wb");
    if (!compressed_file_pointer) {
        printf("Error opening output file: %s\n", compressed_file);
        return;
    }

    /* Write the frequency table to the compressed file */
    fwrite(frequency_table, sizeof(unsigned int), 256, compressed_file_pointer);

    /* Open the uncompressed file */
    FILE *uncompressed_file_pointer = fopen(uncompressed_file, "rb");
    if (!uncompressed_file_pointer) {
        printf("Error opening input file: %s\n", uncompressed_file);
        return;
    }

    /* Write the compressed data to the compressed file */
    write_compressed_data_new(uncompressed_file_pointer, compressed_file_pointer, codes);

    /* Close the file pointers & clean up memory */
    fclose(compressed_file_pointer);
    fclose(uncompressed_file_pointer);
    free_huffman_tree(huffman_tree);
}


/*******************************************************************************
 * Decompresses a file using Huffman coding.
 *
 * inputs:
 * - compressed_file: The file to decompress
 * - uncompressed_file: The file to write decompressed data to
 * outputs:
 * - none
 ******************************************************************************/
void decompress_huffman(const char *compressed_file, const char *uncompressed_file) {

    /* Open the compressed file */
    FILE *compressed_file_pointer = fopen(compressed_file, "rb");
    if (!compressed_file_pointer) {
        printf("Error opening input file: %s\n", compressed_file);
        return;
    }

    /* Read the frequency table from the compressed file */
    unsigned int frequency_table[256];
    fread(frequency_table, sizeof(unsigned int), 256, compressed_file_pointer);

    /* Create Huffman tree */
    HuffmanNode_t *huffman_tree = create_huffman_tree(frequency_table);

    /* Open the uncompressed file */
    FILE *uncompressed_file_pointer = fopen(uncompressed_file, "wb");
    if (!uncompressed_file_pointer) {
        printf("Error opening output file: %s\n", uncompressed_file);
        return;
    }    

    /* Read the compressed data from the compressed file */
    read_compressed_data_new(compressed_file_pointer, uncompressed_file_pointer, huffman_tree);
    
    /* Close the file pointers*/
    fclose(compressed_file_pointer);
    fclose(uncompressed_file_pointer);
    free_huffman_tree(huffman_tree);
}

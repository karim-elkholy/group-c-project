#ifndef COMPRESSION_HUFFMAN_FREQUENCY_TABLE_H
#define COMPRESSION_HUFFMAN_FREQUENCY_TABLE_H

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
void create_frequency_table(const char *input, unsigned int frequency_table[256]);

#endif


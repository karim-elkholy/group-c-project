#include "aes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* TODO - Possible improvements
 * Add support for AES-192 and AES-256
 */

/*******************************************************************************
 * Prints the state matrix.
 * TODO - Remove this function once finished.
 *
 * inputs:
 * - state_matrix - The state matrix to print
 * outputs:
 * - None.
 ******************************************************************************/
void print_state_matrix(state_matrix state)
{
    for (int row_idx = 0; row_idx < 4; row_idx++)
    {
        for (int col_idx = 0; col_idx < 4; col_idx++)
        {
            printf("%02X ", state[row_idx][col_idx]);
        }
        printf("\n");
    }
}

/*******************************************************************************
 * Transforms an array of bytes to a state matrix.
 * The state matrix is stored in column-major order.
 *
 * inputs:
 * - input - The input array of bytes
 * - state - The state matrix to store the result.
 * outputs:
 * - None.
 ******************************************************************************/
void convert_to_state_matrix(const byte *input, state_matrix state)
{
    /* Store the matrix in column-major order.
     * Column-major order = Each column stores the next 4 consecutive bytes.
     */
    int row_idx, col_idx;
    for (row_idx = 0; row_idx < 4; row_idx++)
    {
        for (col_idx = 0; col_idx < 4; col_idx++)
        {
            state[row_idx][col_idx] = input[4 * col_idx + row_idx];
        }
    }
}

/*******************************************************************************
 * Converts a state matrix to an array of bytes.
 *
 * inputs:
 * - state - The state matrix to convert
 * - output - The array of bytes to store the result.
 * outputs:
 * - None.
 ******************************************************************************/
void convert_to_byte_array(state_matrix state, unsigned char *output)
{

    /* Convert the state matrix to a byte array.
     * The state matrix is stored in column-major order.
     * Column-major order = Each column stores the next 4 consecutive bytes.
     */
    int i;
    for (i = 0; i < 16; i++)
    {
        output[i] = state[i % 4][i / 4];
    }
}

/* Lookup table for the AES S-box */
/* Needed by getSubByte() to map the input byte to a substitution byte */
const byte sbox_table[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B,
    0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF,
    0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1,
    0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2,
    0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3,
    0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39,
    0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F,
    0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21,
    0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D,
    0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14,
    0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62,
    0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA,
    0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F,
    0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9,
    0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9,
    0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F,
    0xB0, 0x54, 0xBB, 0x16};

/* Lookup table for the AES Rcon
 * Note: Only 15 values are defined.
 * This table is sufficient for even the largest possible key size(256-bit).
 */
const byte rcon_table[15] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36,
    0x6c, 0xd8, 0xab, 0x4d};

/*******************************************************************************
 * Gets the substitution byte for the given input byte.
 *
 * inputs:
 * - input - The input byte
 * outputs:
 * - The substitution byte.
 *******************************************************************************/
byte getSubByte(byte input)
{
    /* Get the high nibble. This will be the row.
     * Removes bits 0-3.
     */
    unsigned char highNibble = (input & 0xF0) >> 4;

    /* Get the low nibble. This will be the column.
     * Removes bits 4-7.
     */
    unsigned char lowNibble = input & 0x0F;

    /* Return the value at the intersection of the row and column. */
    return sbox_table[highNibble * 16 + lowNibble];
}

/*******************************************************************************
 * Replaces each byte in the word with the corresponding value in the S-box.
 *
 * inputs:
 * - word - The word to edit. Will be modified in place.
 * outputs:
 * - None.
 ******************************************************************************/
void sub_word(byte word[4])
{
    /* Replace each byte with the corresponding value in the S-box. */
    int i;
    for (i = 0; i < 4; i++)
    {
        word[i] = getSubByte(word[i]);
    }
}

/*******************************************************************************
 * Get the number of words in each round key.
 * A word is defined as 32 bits or 4 bytes.
 *
 * The number of words in each round key is determined by the key size.
 * 128-bit/16-byte key = 4 words
 * 192-bit/24-byte key = 6 words
 * 256-bit/32-byte key = 8 words
 *
 * Number of words can manually be calculated using:
 *  1. (key_size_bits / 32)
 *  2. (key_size_bytes / 4)
 *
 * inputs:
 * - key_size - The size of the key
 * outputs:
 * - The number of words in each round key.
 ******************************************************************************/
int get_number_of_words(enum aes_key_size key_size)
{
    /* Return the number of words in the key.
     * 128-bit key = 4 words
     * 192-bit key = 6 words
     * 256-bit key = 8 words
     */
    return key_size / 4;
}

/*******************************************************************************
 * Determine the number of rounds.
 *
 * The number of rounds is determined by the key size.
 *
 * 128-bit key = 10 rounds
 * 192-bit key = 12 rounds
 * 256-bit key = 14 rounds
 *
 * Number of rounds can manually be calculated using:
 *  1. (key_size_bits / 32) + 6
 *  2. (key_size_bytes / 4) + 6
 *
 * inputs:
 * - key_size - The size of the key
 * outputs:
 * - The number of rounds.
 ******************************************************************************/
int get_number_of_rounds(enum aes_key_size key_size)
{
    /* Return the number of rounds. */
    return (key_size / 4) + 6;
}

/*******************************************************************************
 * Helper function.
 * Copies a word from a previous round key to the given array.
 *
 * inputs:
 * - round_key - The round key to copy from.
 * - index - The index of the word to copy.
 *  e.g. 0 for first word, 1 for second word, etc.
 * - output - The array to copy the word to.
 ******************************************************************************/
void copy_word(const byte *round_key, int index, byte *output)
{
    /* Calculate the starting index of the word. */
    int start_index = index * 4;

    /* Copy the word to the given array. */
    int i;
    for (i = 0; i < 4; i++)
    {
        output[i] = round_key[start_index + i];
    }
}

/*******************************************************************************
 * Rotates a word to the left by one byte.
 *
 * inputs:
 * - word - The word to rotate. Will be modified in place.
 * outputs:
 * - None.
 ******************************************************************************/
void rotate_word(byte word[4])
{
    /* Save the first byte. */
    byte temp = word[0];

    /* Shift the last 3 bytes to the left. */
    int i;
    for (i = 0; i < 3; i++)
    {
        word[i] = word[i + 1];
    }

    /* Insert the original first byte at the end of the word. */
    word[3] = temp;
}

byte *key_expansion(const byte *key, enum aes_key_size key_size)
{
    /* Determine how many words are required for each round key.
     * This differs based on the key size.
     */
    int num_words = get_number_of_words(key_size);

    /* Determine the number of round keys to generate.
     * This differs based on the key size.
     */
    int num_rounds = get_number_of_rounds(key_size);

    /* Calculate the total number of words across all the round keys. */
    int total_words = num_rounds * num_words;

    /* Array of state matrices to hold the round keys.
     * Each round key is 16 bytes regardless of the key size.
     */
    byte *round_keys = malloc((num_rounds + 1) * 16);

    /* Copy the original key to the start of the round keys array. */
    memcpy(round_keys, key, key_size);

    /* Iterate until there are no more words to add */
    int i, j;
    for (i = num_words; i < total_words; i++)
    {
        /* Pointer to the current word */
        byte *current_word = round_keys + (i * 4);

        /* Copy the last word
         * This will be used the generate the current word 
         */
        memcpy(current_word, current_word - 4, 4);

        /* Process n words at a time
         * n = 4/6/8 for 128/192/256-bit keys
         */
        if (i % num_words == 0)
        {
            /* Rotate the last word to the left by 1 byte. */
            rotate_word(current_word);

            /* Substitute the bytes with their value from the S-box. */
            sub_word(current_word);

            /* XOR the word with the Rcon value for the current round.
             * Only the first byte of the word is XORed with the Rcon value.
             */
            int current_round = i / num_words;
            current_word[0] ^= rcon_table[current_round];
        }

        /* XOR the current word with the word n words before it 
        * n = 4/6/8 for 128/192/256-bit keys
        */
        byte *prev_word = round_keys + (i - num_words) * 4;
        for (j = 0; j < 4; j++)
        {
            current_word[j] ^= prev_word[j];
        }
    }

    // // Print the round keys
    // printf("Round keys: ");
    // for ( i = 0; i < num_rounds; i++ )
    // {
    //     printf("Round key %d: ", i);
    //     for ( j = 0; j < 16; j++ )
    //     {
    //         printf("%02X ", round_keys[i * 16 + j]);
    //     }
    //     printf("\n");
    // }

    /* Return the round keys. */
    return round_keys;
}

/*******************************************************************************
 * AddRoundKey transformation.
 * XORs the given round key to the given state matrix.
 *
 * inputs:
 * - state - The state matrix to transform. Will be modified in place.
 * - round_key - The round key to XOR with the state matrix.
 * outputs:
 * - None.
 ******************************************************************************/
void add_round_key(state_matrix state, const byte *round_key)
{
    /* XOR the given input with the round key. */
    int row_idx, col_idx;
    for (row_idx = 0; row_idx < 4; row_idx++)
    {
        for (col_idx = 0; col_idx < 4; col_idx++)
        {
            /* XOR corresponding bytes. */
            state[row_idx][col_idx] ^= round_key[col_idx * 4 + row_idx];
        }
    }
}

/*******************************************************************************
 * SubBytes transformation.
 * Map each byte in the state matrix to its corresponding substitution byte.
 *
 * inputs:
 * - state - The state matrix to transform
 * outputs:
 * - None.
 ******************************************************************************/
void sub_bytes(state_matrix state)
{
    /* Substitute each byte in the state matrix. */
    int row_idx, col_idx;
    for (row_idx = 0; row_idx < 4; row_idx++)
    {
        for (col_idx = 0; col_idx < 4; col_idx++)
        {
            /* Map each byte to its corresponding substitution byte. */
            state[row_idx][col_idx] = getSubByte(state[row_idx][col_idx]);
        }
    }
}

/*******************************************************************************
 * ShiftRows transformation.
 * Shifts each row of the state matrix left:
 * - Row 0 is not shifted.
 * - Row 1 is shifted by 1 byte.
 * - Row 2 is shifted by 2 bytes.
 * - Row 3 is shifted by 3 bytes.
 *
 * inputs:
 * - state - The state matrix to transform
 * outputs:
 * - None.
 ******************************************************************************/
void shift_rows(state_matrix state)
{
    /* New state matrix to store the shifted rows. */
    state_matrix shifted_state;

    /* First row is skipped since no bytes are shifted. */
    int row_idx, col_idx;
    for (row_idx = 1; row_idx < 4; row_idx++)
    {
        for (col_idx = 0; col_idx < 4; col_idx++)
        {
            /* Calculate the new position of the byte. */
            int new_col = (col_idx - row_idx + 4) % 4;

            /* Shift the byte to the new position. */
            shifted_state[row_idx][new_col] = state[row_idx][col_idx];
        }

        /* Copy the shifted row back to the original state matrix. */
        for (col_idx = 0; col_idx < 4; col_idx++)
        {
            state[row_idx][col_idx] = shifted_state[row_idx][col_idx];
        }
    }
}

/*******************************************************************************
 * Part of the MixColumns transformation.
 * 'xtime' implementation needed by the MixColumns transformation.
 * Specified in section 4.2 of the FIPS 197 standard.
 *
 * inputs:
 * - x - The byte to multiply.
 * outputs:
 * - The result of the multiplication.
 ******************************************************************************/
byte xtime(byte x)
{
    /*
     * Multiply the given byte by 2
     * Equivalent to shifting the bits to the left by 1.
     */
    byte shifted = x << 1;

    /* Check if x is greater or equal to 128 */
    if (x & 0x80)
    {
        /* The maximum value that a byte can hold is 255.
         * A number greater or equal to 128 multiplied by 2 would exceed 255.
         * This would cause a byte overflow.
         *
         * XORing with 0x1b prevents this overflow.
         * Now the multiplication is within a valid byte range.
         */
        shifted ^= 0x1b;
    }

    /* Return the result of the multiplication. */
    return shifted;
}

/*******************************************************************************
 * Part of the MixColumns transformation.
 * Implements Galois Field (2⁸) multiplication.
 *
 * inputs:
 * - multiplier - The multiplier.
 * - state_byte - The byte from the state matrix to multiply against.
 * outputs:
 * - The result of the multiplication.
 ******************************************************************************/
byte mix_column_gf_multiply(byte multiplier, byte state_byte)
{

    /* Holds the result of the multiplication. */
    byte result = 0;

    /* After a finite number of shifts to the right.
     * The multiplier will eventually become 0.
     * This will break the loop.
     */
    while (multiplier)
    {

        /* Check if the rightmost/least significant bit is 1
         * Same as checking if the number is odd.
         */
        if (multiplier & 1)
        {
            /* Add the state byte to the result.
             * XOR is the equivalent of addition in GF(2^8).
             */
            result ^= state_byte;
        }

        /* Perform the 'xtime' operation on the state byte. */
        state_byte = xtime(state_byte);

        /* Shift the multiplier to the right by 1 bit.
         * This will process the next bit in the multiplier.
         * Equivalent to dividing by 2.
         */
        multiplier >>= 1;
    }

    /* Return the result of the multiplication. */
    return result;
}

/*******************************************************************************
 * MixColumns transformation.
 * The state matrix is multiplied by the mix columns matrix.
 *
 * For each column of the state matrix, the following is done:
 *   row[0] = (m2 • s[0]) ⊕ (m3 • s[1]) ⊕ (m1 • s[2]) ⊕ (m1 • s[3])
 *   row[1] = (m1 • s[0]) ⊕ (m2 • s[1]) ⊕ (m3 • s[2]) ⊕ (m1 • s[3])
 *   row[2] = (m1 • s[0]) ⊕ (m1 • s[1]) ⊕ (m2 • s[2]) ⊕ (m3 • s[3])
 *   row[3] = (m3 • s[0]) ⊕ (m1 • s[1]) ⊕ (m1 • s[2]) ⊕ (m2 • s[3])
 *
 * s = State matrix. s[n] = nth value in the column.
 * m = MixColumns matrix. m1 = 1, m2 = 2, m3 = 3
 * • = Galois Field (2⁸) multiplication
 * ⊕ = XOR.

 * inputs:
 * - state - The state matrix to transform
 * outputs:
 * - None.
 ******************************************************************************/
void mix_columns(state_matrix state)
{
    /* For each column of the state matrix. */
    int col;
    for (col = 0; col < 4; col++)
    {
        /* Get the 4 values in the current column. */
        byte s_0 = state[0][col];
        byte s_1 = state[1][col];
        byte s_2 = state[2][col];
        byte s_3 = state[3][col];

        /* Calculate the mixed column values. */
        state[0][col] =
            mix_column_gf_multiply(2, s_0) ^
            mix_column_gf_multiply(3, s_1) ^
            mix_column_gf_multiply(1, s_2) ^
            mix_column_gf_multiply(1, s_3);

        state[1][col] =
            mix_column_gf_multiply(1, s_0) ^
            mix_column_gf_multiply(2, s_1) ^
            mix_column_gf_multiply(3, s_2) ^
            mix_column_gf_multiply(1, s_3);

        state[2][col] =
            mix_column_gf_multiply(1, s_0) ^
            mix_column_gf_multiply(1, s_1) ^
            mix_column_gf_multiply(2, s_2) ^
            mix_column_gf_multiply(3, s_3);

        state[3][col] =
            mix_column_gf_multiply(3, s_0) ^
            mix_column_gf_multiply(1, s_1) ^
            mix_column_gf_multiply(1, s_2) ^
            mix_column_gf_multiply(2, s_3);
    }
}

/*******************************************************************************
 * AES encryption function.
 *
 * inputs:
 * - input - The input to encrypt. Must be 16 bytes long.
 * - key - The 128, 192, or 256 bit key to use for encryption.
 * - output - The output to store the encrypted data.
 * outputs:
 * - The encrypted output.
 ******************************************************************************/
byte *aes_encrypt(const byte *input, const byte *key, int key_size, byte *output)
{
    /* Get the keys for each round. */
    byte *round_keys = key_expansion(key, key_size);

    /* Convert the input to a state matrix. */
    state_matrix input_state;
    convert_to_state_matrix(input, input_state);

    /* Add the initial round key to the input. */
    add_round_key(input_state, round_keys);

    /* Loop based on the number of rounds 
     * Number of rounds depends on the key size.
     * 10/12/14 rounds for 128/192/256 bit keys.
    */
    int num_rounds = get_number_of_rounds(key_size);
    for (int i = 1; i < num_rounds + 1; i++)
    {
        /* SubBytes transformation. */
        sub_bytes(input_state);

        /* ShiftRows transformation. */
        shift_rows(input_state);
      
        /* If this is not the last round, mix the columns. */
        if (i != num_rounds)
        {
            /* MixColumns transformation. */
            mix_columns(input_state);
        }
        
        /* Add the round key. */
        add_round_key(input_state, round_keys + (i * 16));
    }

    /* Free the round keys. */
    free(round_keys);

    /* Convert the output state matrix to a byte array. */
    convert_to_byte_array(input_state, output);

    return output;
}

aes_encrypted_result aes_encrypt_bytes(const byte *input, int input_size, const byte *key, int key_size)
{

    /* Assume key size is 128 bits. */
    if (key_size != 16 && key_size != 24 && key_size != 32)
    {
        printf("Key size: %d\n", key_size);
        printf("Invalid key size\n");
        exit(0);
    }

    /* Determine the length of padding needed for the last input block. */
    int padding_length = 16 - (input_size % 16);

    /* Split the input into blocks of 16 bytes.
     * An extra block will be needed if the input is a multiple of 16
     */
    int num_blocks = input_size / 16 + 1;

    /* Holds the encrypted output. */
    byte *output = (byte *)malloc(num_blocks * 16 * sizeof(byte));

    /* Encrypt the input in 16 byte increments. */
    int i;
    for (i = 0; i < num_blocks - 1; i++)
    {
        aes_encrypt(input + (i * 16), key, key_size, output + (i * 16));
    }

    /* Create the final block with PKCS#7 padding. */
    byte padded_block[16];
    memset(padded_block, padding_length, 16);

    /* Copy the remaining data to the padded block. */
    memcpy(padded_block, input + (num_blocks - 1) * 16, input_size % 16);

    /* Encrypt the last block. */
    aes_encrypt(padded_block, key, key_size, output + ((num_blocks - 1) * 16));

    // NOTE this uses ECB mode which is different from CBC mode

    /* Create the result struct. */
    struct aes_encrypted_result result;
    result.bytes = output;
    result.size = num_blocks * 16;

    /* Return the encrypted output. */
    return result;
}

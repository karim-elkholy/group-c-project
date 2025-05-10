#include "aes/aes_operations.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aes/maths/gf.h"

/* Lookup table for the AES S-box */
/* Needed by getSubByte() to map the input byte to a substitution byte */
const unsigned char sbox_table[256] = {
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


/* Inverse lookup table for the AES S-box */
/* Needed by getInvSubByte() to map the input byte to a substitution byte */
const unsigned char inv_sbox_table[256] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 
    0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44,
    0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b,
    0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
    0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc,
    0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57,
    0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
    0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03,
    0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce,
    0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
    0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e,
    0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe,
    0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
    0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f,
    0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c,
    0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 
    0x55, 0x21, 0x0c, 0x7d
};

/* Lookup table for the AES Rcon
 * Note: Only 15 values are defined.
 * This table is sufficient for even the largest possible key size(256-bit).
 */
const unsigned char rcon_table[15] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36,
    0x6c, 0xd8, 0xab, 0x4d};

/*******************************************************************************
 * Gets the substitution byte for the given input byte.
 *
 * inputs:
 * - input - The input byte
 * outputs:
 * - The substitution byte.
 ******************************************************************************/
unsigned char getSubByte(unsigned char input)
{
    /* Get the high nibble. This will be the row.
     * Removes bits 0-3.
     */
    unsigned char highNibble = (input & 0xF0) >> 4;

    /* Get the low nibble. This will be the column.
     * Removes bits 4-7.
     */
    unsigned char lowNibble = input & 0x0F;

    /* Return the substitution byte. */
    return sbox_table[highNibble * 16 + lowNibble];
}


/*******************************************************************************
 * Gets the inverse substitution byte for the given input byte.
 *
 * inputs:
 * - input - The input byte
 * outputs:
 * - The inverse substitution byte.
 ******************************************************************************/
unsigned char getInvSubByte(unsigned char input)
{
    /* Get the high nibble. This will be the row.
     * Removes bits 0-3.
     */
    unsigned char highNibble = (input & 0xF0) >> 4;

    /* Get the low nibble. This will be the column.
     * Removes bits 4-7.
     */
    unsigned char lowNibble = input & 0x0F;

    /* Return the inverse substitution byte. */
    return inv_sbox_table[highNibble * 16 + lowNibble];
}

/*******************************************************************************
 * Rotates a word to the left by one byte.
 *
 * inputs:
 * - word - The word to rotate. Will be modified in place.
 * outputs:
 * - None.
 ******************************************************************************/
void rotate_word(unsigned char word[4])
{
    /* Save the first byte. */
    unsigned char temp = word[0];

    /* Shift the last 3 bytes to the left. */
    int i;
    for (i = 0; i < 3; i++)
    {
        word[i] = word[i + 1];
    }

    /* Insert the original first byte at the end of the word. */
    word[3] = temp;
}

/*******************************************************************************
 * Replaces each byte in the word with the corresponding value in the S-box.
 *
 * inputs:
 * - word - The word to edit. Will be modified in place.
 * outputs:
 * - None.
 ******************************************************************************/
void sub_word(unsigned char word[4])
{
    /* Replace each byte with the corresponding value in the S-box. */
    int i;
    for (i = 0; i < 4; i++)
    {
        word[i] = getSubByte(word[i]);
    }
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
void add_round_key(unsigned char state[4][4], const unsigned char *round_key)
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
 * Map each byte in the state matrix to its corresponding substitution 
 * byte.
 *
 * inputs:
 * - state - The state matrix to transform
 * outputs:
 * - None.
 ******************************************************************************/
void sub_bytes(unsigned char state[4][4])
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
 * InvSubBytes transformation.
 * Map each byte in the state matrix to its corresponding inverse substitution 
 * byte.
 *
 * inputs:
 * - state - The state matrix to transform
 * outputs:
 * - None.
 ******************************************************************************/
void inv_sub_bytes(unsigned char state[4][4])
{
    /* Substitute each byte in the state matrix. */
    int row_idx, col_idx;
    for (row_idx = 0; row_idx < 4; row_idx++)
    {
        for (col_idx = 0; col_idx < 4; col_idx++)
        {
            /* Map each byte to its corresponding inverse substitution byte. */
            state[row_idx][col_idx] = getInvSubByte(state[row_idx][col_idx]);
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
void shift_rows(unsigned char state[4][4])
{
    /* New state matrix to store the shifted rows. */
    unsigned char shifted_state[4][4];

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
 * InvShiftRows transformation.
 * Shifts each row of the state matrix right:
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
void inv_shift_rows(unsigned char state[4][4])
{
    /* New state matrix to store the shifted rows. */
    unsigned char shifted_state[4][4];

    /* First row is skipped since no bytes are shifted. */
    int row_idx, col_idx;
    for (row_idx = 1; row_idx < 4; row_idx++)
    {
        for (col_idx = 0; col_idx < 4; col_idx++)
        {
            /* Calculate the new position of the byte. */
            int new_col = (col_idx + row_idx) % 4;

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

unsigned char gf_multiply_2_8(const unsigned char a, const unsigned char b)
{
    /* Create the irreducible polynomial for GF(2^8) */
    unsigned char irreducible[1] = {0x1B};

    /* GF(2^8) only uses 8 bits so only 1 byte is used */
    unsigned char a_array[1] = {a};
    unsigned char b_array[1] = {b};

    /* Perform GF(2^8) multiplication */
    unsigned char result[1] = {0};
    galois_multiply(a_array, b_array, 1, irreducible, result);

    /* Return the first byte of the result */
    return result[0];
}

void gf_multiply_2_128(const unsigned char a[16], const unsigned char b[16],
    unsigned char result[16])
{
    /* Create the irreducible polynomial */
    unsigned char irreducible[16] = {0};
    irreducible[0] = 0xe1;

    /* Perform the GF(2^128) multiplication */
    galois_multiply(a, b, 16, irreducible, result);

    /*
    Intel
    To reduce a 256-bit carry-less product modulo a polynomial g of degree 128,
     we first split it into two 128-bit halves.
     The least significant half is simply XOR-ed with the final remainder
      (since the degree of g is 128).
      For the most significant part,
      we develop an algorithm that realizes division via two multiplications.
       This algorithm can be seen as an extension of the Barrett reduction algorithm to modulo-2
       arithmetic,
       or as an extension of the Feldmeier CRC generation algorithm
       to dividends and divisors of arbitrary size.

     If the product is greater than 255 we "reduce" it.
    Reduction means we divide polynomials by the irreducible polynomial.
    This division will give us a quotient and a remainder.
    The remainder is the reduced result.
    The quotient is discarded.
      */
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
void mix_columns(unsigned char state[4][4])
{
    /* For each column of the state matrix. */
    int col;
    for (col = 0; col < 4; col++)
    {
        /* Get the 4 values in the current column. */
        unsigned char s_0 = state[0][col];
        unsigned char s_1 = state[1][col];
        unsigned char s_2 = state[2][col];
        unsigned char s_3 = state[3][col];

        /* Calculate the mixed column values. */
        state[0][col] =
            gf_multiply_2_8(2, s_0) ^
            gf_multiply_2_8(3, s_1) ^
            gf_multiply_2_8(1, s_2) ^
            gf_multiply_2_8(1, s_3);

        state[1][col] =
            gf_multiply_2_8(1, s_0) ^
            gf_multiply_2_8(2, s_1) ^
            gf_multiply_2_8(3, s_2) ^
            gf_multiply_2_8(1, s_3);

        state[2][col] =
            gf_multiply_2_8(1, s_0) ^
            gf_multiply_2_8(1, s_1) ^
            gf_multiply_2_8(2, s_2) ^
            gf_multiply_2_8(3, s_3);

        state[3][col] =
            gf_multiply_2_8(3, s_0) ^
            gf_multiply_2_8(1, s_1) ^
            gf_multiply_2_8(1, s_2) ^
            gf_multiply_2_8(2, s_3);
    }
}
 
void inv_mix_columns(unsigned char state[4][4])
{
    /* For each column of the state matrix. */
    int col;
    for (col = 0; col < 4; col++)
    {
        /* Get the 4 values in the current column. */
        unsigned char s_0 = state[0][col];
        unsigned char s_1 = state[1][col];
        unsigned char s_2 = state[2][col];
        unsigned char s_3 = state[3][col];

        /* Calculate the mixed column values. */
        state[0][col] =
            gf_multiply_2_8(14, s_0) ^
            gf_multiply_2_8(11, s_1) ^
            gf_multiply_2_8(13, s_2) ^
            gf_multiply_2_8(9, s_3);

        state[1][col] =
            gf_multiply_2_8(9, s_0) ^
            gf_multiply_2_8(14, s_1) ^
            gf_multiply_2_8(11, s_2) ^
            gf_multiply_2_8(13, s_3);

        state[2][col] =
            gf_multiply_2_8(13, s_0) ^
            gf_multiply_2_8(9, s_1) ^
            gf_multiply_2_8(14, s_2) ^
            gf_multiply_2_8(11, s_3);

        state[3][col] =
            gf_multiply_2_8(11, s_0) ^
            gf_multiply_2_8(13, s_1) ^
            gf_multiply_2_8(9, s_2) ^
            gf_multiply_2_8(14, s_3);
    }
}



#include "aes/core/aes_operations.h"

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
 *******************************************************************************/
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

    /* Return the value at the intersection of the row and column. */
    return sbox_table[highNibble * 16 + lowNibble];
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
 * Map each byte in the state matrix to its corresponding substitution byte.
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
 * Part of the MixColumns transformation.
 * 'xtime' implementation needed by the MixColumns transformation.
 * Specified in section 4.2 of the FIPS 197 standard.
 *
 * inputs:
 * - x - The byte to multiply.
 * outputs:
 * - The result of the multiplication.
 ******************************************************************************/
unsigned char xtime(unsigned char x)
{
    /*
     * Multiply the given byte by 2
     * Equivalent to shifting the bits to the left by 1.
     */
    unsigned char shifted = x << 1;

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
unsigned char mix_column_gf_multiply(unsigned char multiplier, unsigned char state_byte)
{

    /* Holds the result of the multiplication. */
    unsigned char result = 0;

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

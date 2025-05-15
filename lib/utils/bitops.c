
#include <stdio.h>
#include <stdlib.h>

#include "utils/bitops.h"

/*******************************************************************************
 * XORs two array of bytes.
 * byte = unsigned char
 *
 * inputs:
 * - result - The result of the XOR.
 * - x - The first array of bytes.
 * - y - The second array of bytes.
 * - n_bytes - Length(in bytes) of 'x' and 'y' arrays.
 * outputs:
 * - None.
 ******************************************************************************/
void xor_bytes(
    const unsigned char *x, const unsigned char *y, 
    int n_bytes, unsigned char *result)
{

    /* Iterate through each byte in the given numbers. */
    int i;
    for (i = 0; i < n_bytes; i++)
    {
        result[i] = x[i] ^ y[i];
    }
}

/*******************************************************************************
 * Shifts an array of bytes to the right by one-byte.
 *
 * inputs:
 * - result - The result of the shift.
 * - x - The array of bytes to shift.
 * - n_bytes - Length(in bytes) of 'x' array.
 * outputs:
 * - None.
 ******************************************************************************/
void shift_bytes_right(unsigned char *result, const unsigned char *x, int n_bytes)
{
    unsigned char carry = 0;

    /* For each byte in the array */
    int i;
    for (i = 0; i < n_bytes; i++)
    {
        /* Get the least significant bit of the current byte */
        unsigned char new_carry = x[i] & 1;

        /* Shift the current byte to the right by 1 */
        result[i] = (x[i] >> 1) | (carry << 7);

        /* Update the carry */
        carry = new_carry;
    }
}

/*******************************************************************************
 * Shifts a byte to the left by n-bits.
 *
 * inputs:
 * - byte - The byte to shift.
 * - n_bits - The number of bits to shift.
 * outputs:
 * - The shifted byte.
 ******************************************************************************/
unsigned char shift_byte_left(unsigned char byte, int n_bits) {

    /* If n_bits is outside an acceptable range */
    if (n_bits < 0 || n_bits > 7) {
        printf("Error: Cannot shift byte left by %d bits\n", n_bits);
        exit(1);
    }

    /* Shift the byte to the left by n_bits */
    return byte << n_bits;
}

/*******************************************************************************
 * Shifts an array of bytes to the left by one-byte.
 *
 * inputs:
 * - result - The result of the shift.
 * - x - The array of bytes to shift.
 * - n_bytes - Length(in bytes) of 'x' array.
 * outputs:
 * - None.
 ******************************************************************************/
void shift_bytes_left(unsigned char *result, const unsigned char *x, int n_bytes)
{
    /* For each byte in the array */
    int i;
    for (i = 0; i < n_bytes - 1; i++)
    {
        result[i] = (x[i] << 1) | (x[i + 1] >> 7);
    }
    result[n_bytes - 1] = x[n_bytes - 1] << 1;
}

/*******************************************************************************
 * Get the most significant bit from the given index.
 * Example:
 * unsigned char byte = 'A';
 * Binary form: 01000001
 * Index: 7
 * 
 * The bit 1 will be returned.
 * 
 * inputs:
 * - byte - The byte.
 * - index - The index of the byte to retrieve.
 * outputs:
 * - The requested bit..
 ******************************************************************************/
unsigned char get_bit_msb(unsigned char byte, int index ) {
    /* If index is outside an acceptable range */
    if (index < 0 || index > 7) {
        printf("Error: Index %d out of range\n", index);
        exit(1);
    }

    /* Return the requested bit */
    unsigned char bit = byte >> (7 - index) & 0x01;
    return bit;
}

/*******************************************************************************
 * Append a bit to the end of a byte.
 * Example:
 * Binary form(before): 01000001
 * Bit to append: 0
 * Binary form(after): 10000010
 * 
 * inputs:
 * - byte - The byte.
 * - bit - The bit to append.
 * outputs:
 * - The updated byte.
 ******************************************************************************/
unsigned char append_bit(unsigned char byte, unsigned char bit) {
    /* Shift the byte to the left by 1 */
    /* The most significant bit will be removed */
    /* Eg. 01000001 -> 10000010 */
    unsigned char new_byte = (byte << 1);
    
    /* Add the bit to the end of the byte */
    /* This will overwrite the least significant bit */
    new_byte |= (bit & 0x01);

    /* Return the new byte*/
    return new_byte;
}
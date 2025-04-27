
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

#include <string.h>
#include <stdlib.h>

#include "aes/maths/gf.h"
#include "utils/bitops.h"


/*******************************************************************************
 * Multiplies two numbers using the Galois Field.
 * Needed by AES for GF(2^8) & GF(2^128).
 *
 * inputs:
 * - result - The result of the multiplication.
 * - x - The first number. Stored as an array of bytes(unsigned char).
 * - y - The second number. Stored as an array of bytes(unsigned char).
 * - n_bytes - Length(in bytes) of 'x' and 'y' arrays.
 * - irreducible - The irreducible polynomial.
 * outputs:
 * - None.
 ******************************************************************************/
void galois_multiply(
    const unsigned char *x, const unsigned char *y,
    int n_bytes,
    const unsigned char *irreducible,
    unsigned char *output
)
{
    /* Stores the result of the multiplication 
    * Should initially be all zeros.
    */
    unsigned char *result = (unsigned char *)calloc(
        n_bytes, sizeof(unsigned char)
    );

    /* Copy of 'x' to work with */
    unsigned char x_copy[n_bytes];
    memcpy(x_copy, x, n_bytes);

    /* Determine the total number of bits in the input polynomials */
    int total_bits = n_bytes * 8;

    /* Iterate through all the bits of the input */
    int i;
    for (i = 0; i < total_bits; i++)
    {
        /* Get the byte index and bit index */
        int byte_idx = i / 8;
        int bit_idx = (n_bytes <= 1) ? (i % 8)        /* Start from LSB for GF(2^8) */
                                     : (7 - (i % 8)); /* Start from MSB for GF(2^9 or greater) */

        /* If the current bit of Y is 1 */
        if (y[byte_idx] & (1 << bit_idx))
        {
            /* XOR the result with the copy of x 
            * This is equivalent to adding x to the result in GF. 
            */
            xor_bytes(result, x_copy, n_bytes, result);
        }

        /* Save edge bit before shifting.
         * Edge bit can mean either MSB(leftmost bit) or LSB(rightmost bit).
         * MSB: for GF(2^8)
         * LSB: for GF(2^9 or greater)
         */
        int edge_bit = (n_bytes <= 1) ? (x_copy[0] & 0x80) : (x_copy[n_bytes - 1] & 1);

        /* Direction to shift is based on field size
         * Shift left for GF(2^8)
         */
        if (n_bytes <= 1)
        {
            /* Shift the last bit of V left by 1 */
            shift_bytes_left(x_copy, x_copy, n_bytes);
        }
        else
        {
            /* Shift all the bits of V to the right by 1 */
            shift_bytes_right(x_copy, x_copy, n_bytes);
        }

        /* If the edge bit is set aka set to 1 */
        if (edge_bit)
        {
            /* XOR x with the irreducible polynomial
             */
            xor_bytes(x_copy, irreducible, n_bytes, x_copy);
        }
    }

    /* Copy the result to the output */
    memcpy(output, result, n_bytes);

    /* Free the result since it has been copied to the output */
    free(result);
}
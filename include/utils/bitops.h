#ifndef UTILS_BITOPS_H
#define UTILS_BITOPS_H

void xor_bytes(
    const unsigned char *x, const unsigned char *y, 
    int n_bytes, unsigned char *result
);
void shift_bytes_right(
    unsigned char *result, const unsigned char *x, int n_bytes
);

void shift_bytes_left(
    unsigned char *result, const unsigned char *x, int n_bytes
);


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
unsigned char get_bit_msb(unsigned char byte, int index );


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
unsigned char append_bit(unsigned char byte, unsigned char bit);

/*******************************************************************************
 * Shifts a byte to the left by n-bits.
 *
 * inputs:
 * - byte - The byte to shift.
 * - n_bits - The number of bits to shift.
 * outputs:
 * - The shifted byte.
 ******************************************************************************/
unsigned char shift_byte_left(unsigned char byte, int n_bits);
#endif




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

#endif




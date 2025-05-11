#ifndef AES_MATHS_GF_H
#define AES_MATHS_GF_H

void galois_multiply(
    const unsigned char *x, const unsigned char *y,
    int n_bytes,
    const unsigned char *irreducible,
    unsigned char *result
);

#endif


#ifndef AES_OPERATIONS_H
#define AES_OPERATIONS_H

/* Lookup table for the AES S-box */
/* Needed by getSubByte() to map the input byte to a substitution byte */
extern const unsigned char sbox_table[256];

/* Lookup table for the AES Rcon */
extern const unsigned char rcon_table[15];

/* KeySchedule-related operations */
void rotate_word(unsigned char word[4]);
void sub_word(unsigned char word[4]);

/* Math operations */
unsigned char *carryless_mul(
    const unsigned char *a, const unsigned char *b, int n_bytes);

/* Encryption-related operations */
void add_round_key(
    unsigned char state[4][4],
    const unsigned char *round_key
);
void sub_bytes(unsigned char state[4][4]);
void shift_rows(unsigned char state[4][4]);
void mix_columns(unsigned char state[4][4]);

/* Galois Field operations */
unsigned char gf_multiply_2_8(const unsigned char a, const unsigned char b);
void gf_multiply_2_128(
    const unsigned char a[16],
    const unsigned char b[16],
    unsigned char result[16]
);

#endif
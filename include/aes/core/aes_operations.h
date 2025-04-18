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

/* Encryption-related operations */
void add_round_key(unsigned char state[4][4], const unsigned char *round_key);
void sub_bytes(unsigned char state[4][4]);
void shift_rows(unsigned char state[4][4]);
void mix_columns(unsigned char state[4][4]);



#endif

#ifndef AES_H
#define AES_H

/* An unsigned char is a byte */
typedef unsigned char byte;

typedef struct aes_encrypted_result {
    byte *bytes;
    int size;
} aes_encrypted_result;

/* Helper functions */
char *convert_bytes_to_hex_string(const byte *input, int input_size);
byte *convert_hex_string_to_bytes(const char *input);

/* Main encrypt/decrypt functions */
/*******************************************************************************
 * AES encryption function.
 *
 * inputs:
 * - input - The input to encrypt. Must be 16 bytes long.
 * - key - The 128, 192, or 256 bit key to use for encryption.
 * - key_size - The size of the key.
 * - output - The output to store the encrypted data.
 * outputs:
 * - None.
*******************************************************************************/
void aes_encrypt_block(const byte *input, const byte *key, int key_size, byte *output);
void aes_decrypt_block(const byte *input, const byte *key, int key_size, byte *output);
aes_encrypted_result *aes_encrypt_bytes(const byte *input, int input_size, const byte *key, int key_size);

#endif
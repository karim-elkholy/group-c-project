
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
void aes_encrypt(const byte *input, const byte *key, int key_size, byte *output);
aes_encrypted_result *aes_encrypt_bytes(const byte *input, int input_size, const byte *key, int key_size);

#endif
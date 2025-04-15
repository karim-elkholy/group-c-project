
// Prevent multiple inclusions.
#ifndef AES_H
#define AES_H

// Valid AES key sizes(in bits)
enum aes_key_size
{
    AES_128_KEY_SIZE = 128,
    AES_192_KEY_SIZE = 192,
    AES_256_KEY_SIZE = 256
};

/* An unsigned char is a byte */
typedef unsigned char byte;

typedef struct aes_encrypted_result {
    byte *bytes;
    int size;
} aes_encrypted_result;

// Main encrypt/decrypt functions
byte *aes_encrypt(const byte *input, const byte *key, int key_size, byte *output);
aes_encrypted_result *aes_encrypt_bytes(const byte *input, int input_size, const byte *key, int key_size);

#endif
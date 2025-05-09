
#ifndef AES_GCM_H
#define AES_GCM_H

/* Holds data encrypted with AES-GCM */
struct aes_gcm_encrypted_data {
    unsigned char *ciphertext;
    int ciphertext_length;

    /* The nonce used for the encryption */
    unsigned char nonce[12];

    /* The authentication tag for the ciphertext.
    * Used to verify the integrity of the ciphertext.
    */
    unsigned char tag[16];
};
typedef struct aes_gcm_encrypted_data aes_gcm_encrypted_data_t;

/*******************************************************************************
 * Encrypts the input using AES-GCM.
 *
 * inputs:
 * - input - The input to encrypt.
 * - input_size - The size of the input.
 * - key - The key to use for the encryption.
 * - key_size - The size of the key.
 * - aad - The additional authentication data.
 * - aad_length - The length of the additional authentication data.
 * - nonce - The nonce to use for the encryption. Must be 12 bytes.
 *
 * outputs:
 * - The ciphertext.
 ******************************************************************************/
aes_gcm_encrypted_data_t *aes_encrypt_gcm(
    const unsigned char *input, 
    int input_size, 
    const unsigned char *key, 
    int key_size,
    const unsigned char *aad,
    int aad_length,
    const unsigned char *nonce);
    
/*******************************************************************************
 * Encrypts the input using AES-GCM.
 *
 * inputs:
 * - input - The input to encrypt.
 * - input_size - The size of the input.
 * - key - The key to use for the encryption.
 * - key_size - The size of the key.
 * - aad - The additional authentication data.
 * - aad_length - The length of the additional authentication data.
 *
 * outputs:
 * - The encrypted data.
 ******************************************************************************/
aes_gcm_encrypted_data_t *aes_encrypt_gcm_bytes(
    const unsigned char *input,
    int input_size,
    const unsigned char *key,
    int key_size,
    const unsigned char *aad,
    int aad_length);

#endif


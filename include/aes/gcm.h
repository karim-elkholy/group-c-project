
#ifndef AES_GCM_H
#define AES_GCM_H

/* Holds data encrypted with AES-GCM */
struct aes_gcm_data {
    /* Stores the output of the encryption/decryption & its length */
    unsigned char *output;
    int output_length;

    /* The authentication tag for the encryption/decryption.
    * Used to verify the integrity of the data.
    */
    unsigned char tag[16];
};
typedef struct aes_gcm_data aes_gcm_data_t;


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
aes_gcm_data_t *aes_encrypt_gcm(
    const unsigned char *plaintext, 
    int plaintext_size, 
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
aes_gcm_data_t *aes_encrypt_gcm_bytes(
    const unsigned char *input,
    int input_size,
    const unsigned char *key,
    int key_size,
    const unsigned char *aad,
    int aad_length);

/*******************************************************************************
 * Decrypts the input using AES-GCM.
 *
 * inputs:
 * - input - The input to decrypt.
 * - input_size - The size of the input.
 * - key - The key to use for the decryption.
 * - key_size - The size of the key.
 * - aad - The additional authentication data.
 * - aad_length - The length of the additional authentication data.
 * - nonce - The nonce to use for the decryption. Must be 12 bytes.
 * - auth_tag - The 16 byte authentication tag verifying ciphertext integrity.
 *
 * outputs:
 * - The decrypted data.
 ******************************************************************************/
aes_gcm_data_t *aes_decrypt_gcm(
    const unsigned char *ciphertext,
    int ciphertext_size,
    const unsigned char *key,
    int key_size,
    const unsigned char *aad,
    int aad_length,
    const unsigned char *nonce,
    const unsigned char *auth_tag);

#endif


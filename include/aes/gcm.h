
#ifndef AES_GCM_H
#define AES_GCM_H

/* Holds data encrypted with AES-GCM */
struct aes_gcm_encrypted_data {
    unsigned char *ciphertext;
    int ciphertext_length;

    /* The nonce used for the encryption */
    unsigned char nonce[12];

    /* The tag 
    * 12 bytes is used to follow the recommended standard.
    */
    unsigned char tag[16];
};
typedef struct aes_gcm_encrypted_data aes_gcm_encrypted_data_t;

aes_gcm_encrypted_data_t *aes_encrypt_gcm(
    const unsigned char *input, 
    int input_size, 
    const unsigned char *key, 
    int key_size,
    const unsigned char *nonce);
    
void aes_encrypt_gcm_bytes(
    const unsigned char *input,
    int input_size,
    const unsigned char *key,
    int key_size);

#endif


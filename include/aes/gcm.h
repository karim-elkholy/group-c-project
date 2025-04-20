
#ifndef AES_GCM_H
#define AES_GCM_H


void aes_encrypt_gcm(
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


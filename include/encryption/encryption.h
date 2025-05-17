#ifndef ENCRYPTION_ENCRYPTION_H
#define ENCRYPTION_ENCRYPTION_H

/*******************************************************************************
 * Encrypts a file using AES-GCM.
 *
 * inputs:
 * - plaintext_file - The file to encrypt.
 * - encrypted_file - The file to write the encrypted data to.
 * - key - The key to use for the encryption.
 * - key_size - The size of the key.
 * - aad - The additional authentication data.
 * - aad_length - The length of the additional authentication data.
 * - nonce - The nonce. Must be 12 bytes.
 *
 * outputs:
 * - None.
 ******************************************************************************/
void aes_gcm_encrypt_file(
    const char *plaintext_file,
    const char *encrypted_file,
    const unsigned char *key,
    int key_size,
    const unsigned char *aad,
    int aad_length,
    const unsigned char *nonce);

/*******************************************************************************
 * Decrypts a file using AES-GCM.
 *
 * inputs:
 * - encrypted_file - The file to decrypt.
 * - decrypted_file - The file to write the decrypted data to.
 * - key - The key to use for the decryption.
 * - key_size - The size of the key.
 * - aad - The additional authentication data.
 * - aad_length - The length of the additional authentication data.
 * - nonce - The nonce. Must be 12 bytes.
 *
 * outputs:
 * - None.
 ******************************************************************************/
void aes_gcm_decrypt_file(
    const char *encrypted_file,
    const char *decrypted_file,
    const unsigned char *key,
    int key_size,
    const unsigned char *aad,
    int aad_length,
    const unsigned char *nonce);

#endif


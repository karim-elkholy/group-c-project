#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils/random.h"
#include "encryption/aes/core.h"
#include "encryption/aes/keyschedule.h"
#include "encryption/aes/operations.h"
#include "encryption/aes/gcm.h"


/* Context needed during GCM encryption */
struct aes_gcm_context
{
    /*** Input/Outputs ***/
    /* The nonce */
    unsigned char nonce[12];

    /* Holds the AAD(Additional Authentication Data) & its length */
    const unsigned char *aad;
    int aad_length;

    /* Holds the key & its size */
    const unsigned char *key;
    int key_size;

    /*** Internal Variables ***/

    /* The counter block */
    unsigned char counter_block[16];

    /* The number of blocks needed for the plaintext */
    /* int num_blocks_input; */

    /* The number of blocks needed for the AAD */
    /* int num_blocks_aad; */

    /* Holds the bytes processed */
    int bytes_processed;

    /* Holds the hash subkey
    * Used in GHASH calculations.
    */
    unsigned char hash_subkey[16];

    /* Holds the ghash */
    unsigned char ghash[16];

    /* Holds y0 aka the first counter block encrypted with the key 
    * Used in tag generation.
    */
    unsigned char y0[16];

};
typedef struct aes_gcm_context aes_gcm_context_t;

/*******************************************************************************
 * Generates a 12-byte random sequence of bytes.
 * This will be the nonce for the GCM encryption.
 *
 * inputs:
 * - None.
 *
 * outputs:
 * - A 12-byte random sequence of bytes.
 ******************************************************************************/
unsigned char *aes_gcm_generate_nonce()
{
    /* Create a 12-byte random sequence of bytes
     * This will be the nonce for the GCM encryption
     */
    return random_bytes(12);
}

/*******************************************************************************
 * Initializes the GCM context for encryption.
 * Used if the caller does not want to provide a nonce.
 *
 * inputs:
 * - input - The input to transform. E.g. plaintext or ciphertext.
 * - input_size - The size of the input.
 * - key - The key to use for the transformation.
 * - key_size - The size of the key.
 * - nonce - The nonce to use for the transformation.
 *
 * outputs:
 * - The GCM context.
 ******************************************************************************/
aes_gcm_context_t *aes_gcm_context_init(
    const unsigned char *key,
    int key_size,
    const unsigned char *aad,
    int aad_length,
    const unsigned char *nonce)
{
    /* Allocate memory for the context */
    aes_gcm_context_t *ctx = (aes_gcm_context_t *)malloc(
        sizeof(aes_gcm_context_t));

    /* Set the bytes processed to 0 */
    ctx->bytes_processed = 0;

    /* Set the key and its size */
    ctx->key = key;
    ctx->key_size = key_size;
    /* Set the AAD & its length */
    ctx->aad = aad;
    ctx->aad_length = aad_length;

    /* Initialize the hash subkey 
    * The hash subkey is an 16 byte block of 0s encrypted with the key.
    */
    memset(ctx->hash_subkey, 0, 16);
    aes_encrypt_block(ctx->hash_subkey, ctx->key, ctx->key_size, ctx->hash_subkey);

    /* Add the nonce to this context */
    memcpy(ctx->nonce, nonce, 12);

    /* Initialize the counter block which is 16 bytes long
    * The first 12 bytes of the counter block are the nonce.
    * The last 4 bytes of the counter block are 0x00000001.
    */
    memcpy(ctx->counter_block, nonce, 12); 
    ctx->counter_block[12] = 0x00;
    ctx->counter_block[13] = 0x00;
    ctx->counter_block[14] = 0x00;
    ctx->counter_block[15] = 0x01;

    /* Encrypt the counter block to get y0
    * This is needed to generate the tag.
    */
    aes_encrypt_block(ctx->counter_block, ctx->key, ctx->key_size, ctx->y0);

    /* Return the context */
    return ctx;
}

/*******************************************************************************
 * Determines the number of blocks needed for the given input.
 *
 * inputs:
 * - input_size - The size of the input.
 *
 * outputs:
 * - The number of blocks needed to process the input.
 ******************************************************************************/
int determine_num_blocks(int input_size) {
    return (int) ceil((float) input_size / 16);
}

/*******************************************************************************
 * Generates the ciphertext/plaintext for the current block.
 *
 * inputs:
 * - context - The context to use for the GCM encryption.
 * - input - The input to encrypt.
 * - input_length - The length of the input.
 * - output - The output to write the ciphertext/plaintext to.
 *
 * outputs:
 * - None.
 ******************************************************************************/
void aes_gcm_xor_input_with_keystream(
    aes_gcm_context_t *ctx, 
    const unsigned char *input,
    int input_length,
    unsigned char *keystream_block,
    unsigned char *output
    )
{
  
    /* Get the location to place the ciphertext/plaintext */
    unsigned char *output_offset = output + ctx->bytes_processed;

    /* Number of bytes to process */
    /* By default, assume there are 16 bytes to process */
    int bytes_to_process = 16;

    /* If there are not 16 bytes remaining */
    if (ctx->bytes_processed + 16 > input_length) {
        bytes_to_process = input_length - ctx->bytes_processed;
    }

    /* Process the remaining bytes */
    int i;
    for (i = 0; i < bytes_to_process; i++)
    {
        /* XOR the plaintext with the keystream */
        output_offset[i] = input[i] ^ keystream_block[i];
    }
    /* Update the bytes processed */
    ctx->bytes_processed += bytes_to_process;
}

/*******************************************************************************
 * Generates the keystream block for the GCM encryption.
 * This updates every block.
 * 
 * inputs:
 * - ctx - The context to use for the GCM encryption.
 *
 * outputs:
 * - None.
 ******************************************************************************/
void aes_gcm_create_keystream(
    aes_gcm_context_t *ctx,
    unsigned char *keystream_block
) {

    /* Generate the keystream block */
    aes_encrypt_block(
        ctx->counter_block,
        ctx->key,
        ctx->key_size,
        keystream_block
    );
}

/*******************************************************************************
 * Increments the counter block.
 *
 * inputs:
 * - ctx - The context to use for the GCM encryption.
 *
 * outputs:
 * - None.
 ******************************************************************************/
void aes_gcm_increment_counter_block(aes_gcm_context_t *ctx) {

    /* Indicates whether to add 1 to the current byte
     * By default, this is 1 to increment the rightmost byte.
     */
    int add_one = 1;

    /* Iterate from the rightmost byte to the leftmost byte
     * Only the last 4 bytes are considered.
     */
    int i;
    for ( i = 15; i >= 12; i-- ) {

        /* If 1 should be added to the current byte */
        if ( add_one == 1 ) {

            /* If the leftmost byte is at its max value(255) */
            if ( i == 12 && ctx->counter_block[12] == 0xFF ) {
                /* Exit with an error to prevent overflow */
                /* Source: https://crypto.stackexchange.com/a/31798 */
                printf("[ERROR] Counter block is at its max value\n");
                exit(1);

            /* If any other byte is at its max value(255) */
            } else if (ctx->counter_block[i] == 0xFF ) {

                /* Reset the byte back to 0 */
                ctx->counter_block[i] = 0;

                /* Set the next byte to be incremented */
                add_one = 1;
       
            } else {
                /* Increment the current byte by 1 */
                ctx->counter_block[i] += 1;

                /* Exit the loop since the increment is complete */
                break;
            }
        }
    }
}

/*******************************************************************************
 * Calculates the length block needed for the AES-GCM encryption.
 * length_block = len(A)||len(C)
 * 
 * inputs:
 * - ctx - The context to use for the AES-GCM encryption.
 *
 * outputs:
 * - The length block.
 */
void aes_gcm_calculate_length_block(
    const aes_gcm_context_t *ctx,
    int ciphertext_length,
    unsigned char length_block[16]
) {

    /* Length of associated data in bits */
    unsigned long long len_a = (unsigned long long)ctx->aad_length * 8;
    
    /* Length of ciphertext in bits */
    unsigned long long len_c = (unsigned long long)ciphertext_length * 8;  

    /* Calculate len(A)||len(C) where:
     * len(A) = length of associated data in bits
     * len(C) = length of ciphertext in bits
     * 
     * || = concatenation
     *  byte 0-8 holds len(A)
     *  byte 9-15 holds len(C)
     */
    int i;
    for (i = 0; i < 8; i++) {
        length_block[15 - i] = (len_c >> (i * 8)) & 0xFF;
    }
    for (i = 0; i < 8; i++) {
        length_block[7 - i] = (len_a >> (i * 8)) & 0xFF;
    }
}

void aes_gcm_update_ghash(
    aes_gcm_context_t *ctx, 
    const unsigned char *block, 
    int block_size) {
    
    int i;

    /* XOR the block with the current GHASH value */
    for (i = 0; i < block_size; i++) {
        ctx->ghash[i] ^= block[i];
    }

    /* Perform GF multiplication: xᵢ = (xᵢ₋₁ ⊕ Cᵢ) • H 
    * Cᵢ = encrypted version of the ith block of plaintext.
    * H = hash subkey.
    */
    gf_multiply_2_128(
        ctx->ghash, ctx->hash_subkey, ctx->ghash
    );
}

void b(
    aes_gcm_context_t *ctx, 
    const unsigned char *ciphertext, int ciphertext_length,
    int num_blocks_ciphertext, int num_blocks_aad
    ) {
    /* Initialize GHASH to zero */
    memset(ctx->ghash, 0, 16);

    int i;
    /* First process AAD blocks */
    for (i = 0; i < num_blocks_aad; i++) {

        /* Pointer to the current block's AAD */
        const unsigned char *current_aad_block = ctx->aad 
            + (i * 16);

        /* Calculate how many bytes to process */
        int bytes_to_process = 16;
        if (i == num_blocks_aad - 1) {
            bytes_to_process = ctx->aad_length - (i * 16);
        }

        /* Update the GHASH */
        aes_gcm_update_ghash(ctx, current_aad_block, bytes_to_process);
    }

    /* Then process ciphertext blocks */
    for (i = 0; i < num_blocks_ciphertext; i++) {

        /* Pointer to the current block's ciphertext */
        const unsigned char *current_ciphertext = ciphertext + (i * 16);

        /* Calculate how many bytes to process */
        int bytes_to_process = 16;
        if (i == num_blocks_ciphertext - 1) {
            bytes_to_process = ciphertext_length - (i * 16);
        }

        /* Update the GHASH */
        aes_gcm_update_ghash(ctx, current_ciphertext, bytes_to_process);
    }

    /* Finally process length block */
    unsigned char length_block[16] = {0};
    aes_gcm_calculate_length_block(ctx, ciphertext_length, length_block);
    
    /* Update the GHASH */
    aes_gcm_update_ghash(ctx, length_block, 16);
}


void aes_gcm_calculate_tag(
    aes_gcm_context_t *ctx, 
    const unsigned char *ciphertext, int ciphertext_length,
    int num_blocks_ciphertext, int num_blocks_aad,
    unsigned char tag[16]) {

    /* Calculate the ghash needed for creating the tag */
    b(ctx, ciphertext, ciphertext_length, num_blocks_ciphertext, num_blocks_aad);

    /* XOR the GHash with the nonce */
    int i;
    for (i = 0; i < 16; i++) {
        tag[i] = ctx->ghash[i] ^ ctx->y0[i];
    }
}

/* IF debug level 3 is defined*/
#ifdef DEBUG 
void debug_print(aes_gcm_context_t *ctx, int round) {
    /* Variables needed */
    int i;

    if (round == 0) {
        /* Print the hash key for the first round */
        printf("[DEBUG] Hash key: ");
        for (i = 0; i < 16; i++) printf("%02X", ctx->hash_subkey[i]);
        printf("\n");

        /* Print the first counter block */
        printf("[DEBUG] Y0: ");
        for (i = 0; i < 16; i++) printf("%02X", ctx->counter_block[i]);
        printf("\n");

        /* Print the encrypted counter block for the initial round(round 0) */
        printf("[DEBUG] E(K,Y0): ");
        for (i = 0; i < 16; i++) printf("%02X", ctx->y0[i]);
        printf("\n");
    
    /* For all other rounds*/
    } else {
        /* Print the counter block */
        printf("[DEBUG] Y(%d): ", round);
        for (i = 0; i < 16; i++) printf("%02X", ctx->counter_block[i]);
        printf("\n");

        /* Print the keystream block */
        printf("[DEBUG] E(K,Y0): ");
        for (i = 0; i < 16; i++) printf("%02X", ctx->y0[i]);
        printf("\n");

        /* Print the updated ghash */
        printf("[DEBUG] GHASH %d: ", round);
        for (i = 0; i < 16; i++) printf("%02X", ctx->ghash[i]);
        printf("\n");
    }
}

#endif

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
 * - nonce - The nonce. Must be 12 bytes.
 *
 * outputs:
 * - The ciphertext.
 ******************************************************************************/
aes_gcm_data_t *aes_gcm_encrypt(
    const unsigned char *plaintext, 
    int plaintext_size, 
    const unsigned char *key, 
    int key_size,
    const unsigned char *aad,
    int aad_length,
    const unsigned char *nonce) {

    /* Get the number of blocks needed for the plaintext */
    int num_blocks_plaintext = determine_num_blocks(plaintext_size);

    /* Get the number of blocks needed for the AAD */
    int num_blocks_aad = determine_num_blocks(aad_length);

    /* Initialize the context needed for AES-GCM encryption */
    aes_gcm_context_t *ctx = aes_gcm_context_init(
        key, key_size, aad, aad_length, nonce
    );

    /* Allocate memory for the ciphertext */
    int ciphertext_size = plaintext_size;
    unsigned char *ciphertext = (unsigned char *)malloc(
        ciphertext_size * sizeof(unsigned char));

    /* Iterate through all the blocks */
    int i; 
    for (i = 0; i < num_blocks_plaintext; i++)
    {
        /* Increment the counter block */
        /* Start at 1 because counter block 0(E(K,Y0)) is reserved for the nonce. */
        aes_gcm_increment_counter_block(ctx);

        /* Generate the keystream block for this block */
        unsigned char keystream_block[16];
        aes_gcm_create_keystream(ctx, keystream_block);

        /* XOR the current plaintext block of 16 bytes with the keystream */
        /* Creates the ciphertext for this plaintext block */
        aes_gcm_xor_input_with_keystream(
            ctx, 
            plaintext + (i * 16), 
            plaintext_size,
            keystream_block,
            ciphertext);

        /* Debugging if needed */
        #if defined(DEBUG) && DEBUG_LEVEL == 3
            debug_print(ctx, i);
        #endif
    }
    
    /* Calculate the tag */
    unsigned char tag[16];
    aes_gcm_calculate_tag(ctx, 
        ciphertext, 
        ciphertext_size,
        num_blocks_plaintext, /* Plaintext is always same size as ciphertext */
        num_blocks_aad,
        tag
    );

    /* Debugging if needed */
    #if defined(DEBUG) && DEBUG_LEVEL == 3
        debug_print(ctx, i);
    #endif

    /* Free the memory allocated for the context */
    free(ctx);

    /* Create the output data */
    aes_gcm_data_t *output = (aes_gcm_data_t *)malloc(
        sizeof(aes_gcm_data_t));
    output->output = ciphertext;
    output->output_length = ciphertext_size;

    /* Copy the tag to the output */
    memcpy(output->tag, tag, 16);

    /* Return the encrypted data */
    return output;
}

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
    const unsigned char *nonce) {

    /* Open the plaintext file */
    FILE *plaintext_file_ptr = fopen(plaintext_file, "rb");
    if (plaintext_file_ptr == NULL) {
        printf("[ERROR] Failed to open plaintext file\n");
        return;
    }
    
    /* Open the encrypted file */
    FILE *encrypted_file_ptr = fopen(encrypted_file, "wb");
    if (encrypted_file_ptr == NULL) {
        printf("[ERROR] Failed to open encrypted file\n");
        return;
    }
    
    /* Determine the size of the plaintext file */
    fseek(plaintext_file_ptr, 0, SEEK_END);
    int plaintext_size = ftell(plaintext_file_ptr);
    fseek(plaintext_file_ptr, 0, SEEK_SET);

    /* Read the plaintext file into memory */
    unsigned char *plaintext = (unsigned char *)malloc(plaintext_size);
    fread(plaintext, 1, plaintext_size, plaintext_file_ptr);

    /* Encrypt the plaintext */
    aes_gcm_data_t *encrypted_data = aes_gcm_encrypt(
        plaintext, plaintext_size, key, key_size, aad, aad_length, nonce);

    /* Write the authentication tag to the encrypted file */
    fwrite(encrypted_data->tag, 1, 16, encrypted_file_ptr);

    /* Write the encrypted data to the encrypted file */
    fwrite(encrypted_data->output, 1, encrypted_data->output_length, encrypted_file_ptr);

    /* Free any memory allocated */
    free(plaintext);
    free(encrypted_data);

    /* Close the files */
    fclose(plaintext_file_ptr);
    fclose(encrypted_file_ptr);
}

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
    const unsigned char *nonce) {

    /* Open the decrypted file */
    FILE *decrypted_file_ptr = fopen(decrypted_file, "wb");
    if (decrypted_file_ptr == NULL) {
        printf("[ERROR] Failed to open decrypted file\n");
        return;
    }

    /* Open the encrypted file */
    FILE *encrypted_file_ptr = fopen(encrypted_file, "rb");
    if (encrypted_file_ptr == NULL) {
        printf("[ERROR] Failed to open encrypted file\n");
        return;
    }

    /* Determine the size of the encrypted file */
    fseek(encrypted_file_ptr, 0, SEEK_END);
    int encrypted_file_size = ftell(encrypted_file_ptr);
    fseek(encrypted_file_ptr, 0, SEEK_SET);

    /* Subtract 16 due to the authentication tag */
    encrypted_file_size -= 16;

    /* Read the authentication tag from the file */
    unsigned char auth_tag[16];
    fread(auth_tag, 1, 16, encrypted_file_ptr);

    /* Read the encrypted file into memory */
    unsigned char *encrypted_data = (unsigned char *)malloc(encrypted_file_size);
    fread(encrypted_data, 1, encrypted_file_size, encrypted_file_ptr);
;

    /* Decrypt the encrypted data */
    aes_gcm_data_t *decrypted_data = aes_gcm_decrypt(
        encrypted_data, encrypted_file_size, 
        key, key_size,
        aad, aad_length,
        nonce,
        auth_tag);

    /* Check if the decrypted data is valid */
    if (decrypted_data == NULL) {
        printf("[ERROR] Failed to decrypt data\n");
        return;
    }

    /* Write the decrypted data to the decrypted file */
    fwrite(decrypted_data->output, 1, 
        decrypted_data->output_length, decrypted_file_ptr);

    /* Free any memory allocated */
    free(encrypted_data);
    free(decrypted_data);

    /* Close the files */
    fclose(encrypted_file_ptr);
    fclose(decrypted_file_ptr);
}

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
 * - nonce - The nonce. Must be 12 bytes.
 * - auth_tag - The 16 byte authentication tag verifying ciphertext integrity.
 *
 * outputs:
 * - The decrypted data.
 ******************************************************************************/
aes_gcm_data_t *aes_gcm_decrypt(
    const unsigned char *ciphertext,
    int ciphertext_size,
    const unsigned char *key,
    int key_size,
    const unsigned char *aad,
    int aad_length,
    const unsigned char *nonce,
    const unsigned char *auth_tag) {

    /* Get the number of blocks needed for the ciphertext */
    int num_blocks_ciphertext = determine_num_blocks(ciphertext_size);

    /* Get the number of blocks needed for the AAD */
    int num_blocks_aad = determine_num_blocks(aad_length);

    /* Initialize the context needed for AES-GCM decryption */
    aes_gcm_context_t *ctx = aes_gcm_context_init(
        key, key_size, 
        aad, aad_length,
        nonce
    );

    /* Calculate the tag */
    unsigned char tag[16];
    aes_gcm_calculate_tag(ctx, ciphertext, ciphertext_size, 
        num_blocks_ciphertext, num_blocks_aad, tag);

    /* Check if the tag is valid */
    if (memcmp(auth_tag, tag, 16) != 0) {
        printf("[ERROR] Tag is invalid\n");
        
        /* Print the two different tags */
        printf("[DEBUG] Generated tag: ");
        int temp_i;
        for (temp_i = 0; temp_i < 16; temp_i++) printf("%02X", tag[temp_i]);
        printf("\n");
        printf("[DEBUG] Expected tag: ");
        for (temp_i = 0; temp_i < 16; temp_i++) printf("%02X", auth_tag[temp_i]);
        printf("\n");

        return NULL;
    }

    /* Allocate memory for the plaintext */
    int plaintext_size = ciphertext_size;
    unsigned char *plaintext = (unsigned char *)malloc(
        plaintext_size * sizeof(unsigned char));


    /* Iterate through all the blocks of ciphertext */
    int i; 
    for (i = 0; i < num_blocks_ciphertext; i++)
    {
        /* Increment the counter block */
        aes_gcm_increment_counter_block(ctx);

        /* Generate the keystream block for this block */
        unsigned char keystream_block[16];
        aes_gcm_create_keystream(ctx, keystream_block);

        /* XOR the current ciphertext block of 16 bytes with the keystream */
        /* Creates the plaintext for this ciphertext block */
        aes_gcm_xor_input_with_keystream(
            ctx, 
            ciphertext + (i * 16), ciphertext_size,
            keystream_block,
            plaintext);

        /* Debugging if needed */
        #if defined(DEBUG) && DEBUG_LEVEL == 3
            debug_print(ctx, i);
        #endif
    }

    /* Free the memory allocated for the context */
    free(ctx);

    /* Create the output data */
    aes_gcm_data_t *output = (aes_gcm_data_t *)malloc(
        sizeof(aes_gcm_data_t));
    output->output = plaintext;
    output->output_length = ciphertext_size;

    /* Return the decrypted data */
    return output;
}



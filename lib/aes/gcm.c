#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "portable64/counter.h"
#include "utils/random.h"
#include "aes/core/aes.h"
#include "aes/core/aes_keyschedule.h"
#include "aes/core/aes_operations.h"

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

/* Context needed during GCM encryption */
struct aes_gcm_context
{
    /* The nonce */
    unsigned char nonce[12];

    /* The counter block */
    unsigned char counter_block[16];

    /* The number of blocks needed */
    int num_blocks;

    /* Holds the bytes processed */
    int bytes_processed;

    /* Holds the length of the plaintext */
    int plaintext_length;

    /* Holds the key & its size */
    const unsigned char *key;
    int key_size;

    /* Holds the keystream block */
    unsigned char keystream_block[16];

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

    /* Pointer to the encrypted data */
    aes_gcm_encrypted_data_t *data_encr;
};
typedef struct aes_gcm_context aes_gcm_context_t;

/*******************************************************************************
 * Initializes the counter block for the GCM encryption.
 *
 * inputs:
 * - nonce - The nonce to use for the GCM encryption.
 *
 * outputs:
 * - The counter block.
 ******************************************************************************/
void aes_init_counter_block(
    unsigned char *counter_block, 
    const unsigned char *nonce
)
{
    /* Copy the nonce to the counter block */
    memcpy(counter_block, nonce, 12); 

    /* For nonces of 12 bytes, the last 4 bytes are 0x00000001 */
    counter_block[12] = 0x00;
    counter_block[13] = 0x00;
    counter_block[14] = 0x00;
    counter_block[15] = 0x01;
}

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
 * Initializes the hash subkey for the GCM encryption.
 *
 * inputs:
 * - ctx - The context to use for the GCM encryption.
 *
 * outputs:
 * - None.
 ******************************************************************************/
void aes_gcm_init_hash_subkey(aes_gcm_context_t *ctx) { 

    /* Hash subkey should be all zero */
    memset(ctx->hash_subkey, 0, 16);
    /* Initialize the hash subkey */
    aes_encrypt(ctx->hash_subkey, ctx->key, ctx->key_size, ctx->hash_subkey);
}

/*******************************************************************************
 * Initializes the GCM context for encryption.
 * Used if the caller does not want to provide a nonce.
 *
 * inputs:
 * - input - The input to encrypt.
 * - input_size - The size of the input.
 * - key - The key to use for the encryption.
 * - key_size - The size of the key.
 * - nonce - The nonce to use for the encryption.
 *
 * outputs:
 * - The GCM context.
 ******************************************************************************/
aes_gcm_context_t *aes_gcm_encrypt_init(
    const unsigned char *input,
    int input_size, 
    const unsigned char *key,
    int key_size,
    const unsigned char *nonce)
{
    /* Allocate memory for the context */
    aes_gcm_context_t *ctx = (aes_gcm_context_t *)malloc(
        sizeof(aes_gcm_context_t));

    /* Determine the number of blocks needed
     * Each block will be encrypted in increments of 16 bytes.
     */
    ctx->num_blocks = (int) ceil((float) input_size / 16);

    /* Set the bytes processed to 0 */
    ctx->bytes_processed = 0;

    /* Set the plaintext length */
    ctx->plaintext_length = input_size;
    /* Set the key and its size */
    ctx->key = key;
    ctx->key_size = key_size;

    /* Initialize the hash subkey */
    aes_gcm_init_hash_subkey(ctx);

    /* Copy the nonce */
    memcpy(ctx->nonce, nonce, 12);

    /* Initialize the counter block */
    aes_init_counter_block(ctx->counter_block, ctx->nonce);

    /* Encrypt the counter block to get y0
    * This will be used to generate the tag.
    */
    aes_encrypt(ctx->counter_block, ctx->key, ctx->key_size, ctx->y0);

    /* Initialize the encrypted data */
    ctx->data_encr = (aes_gcm_encrypted_data_t *)malloc(
        sizeof(aes_gcm_encrypted_data_t)
    );
    ctx->data_encr->ciphertext = (unsigned char *)malloc(
        input_size * sizeof(unsigned char));

    /* Return the context */
    return ctx;
}


/*******************************************************************************
 * Generates the ciphertext for the current block of plaintext.
 *
 * inputs:
 * - context - The context to use for the GCM encryption.
 * - plaintext - The plaintext to encrypt.
 * - keystream - The keystream to use for the GCM encryption.
 *
 * outputs:
 * - None.
 ******************************************************************************/
void aes_gcm_create_ciphertext(aes_gcm_context_t *ctx, const unsigned char *input)
{
    /* Get the location to place the ciphertext */
    unsigned char *output = ctx->data_encr->ciphertext + ctx->bytes_processed;

    /* Iterate in 16 byte chunks */
    int i;
    for (i = 0; i < 16; i++)
    {
        /* XOR the plaintext with the keystream */
        output[i] = input[i] ^ ctx->keystream_block[i];
    }
    /* Update the bytes processed */
    ctx->bytes_processed += 16;
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
void aes_gcm_create_keystream(aes_gcm_context_t *ctx) {

    /* Generate the keystream block */
    aes_encrypt(
        ctx->counter_block,
        ctx->key,
        ctx->key_size,
        ctx->keystream_block
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
    unsigned char length_block[16]
) {

    /* Length of associated data in bits */
    unsigned long long len_a = 0;
    
    /* Length of ciphertext in bits */
    unsigned long long len_c = (unsigned long long)ctx->plaintext_length * 8;  

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

/*******************************************************************************
 * Calculates the ghash for the AES-GCM encryption.
 * Ghash is the cumulative hash of the ciphertext and the length block.
 *
 * inputs:
 * - ctx - The context to use for the AES-GCM encryption.
 *
 * outputs:
 * - None.
 ******************************************************************************/
void aes_gcm_calculate_ghash(aes_gcm_context_t *ctx) {

    /* todo -  this function is too long. either shorten it or break it up */
    /* Ghash should initially be all zeros */
    memset(ctx->ghash, 0, 16);

    /* Calculate the length block */
    unsigned char length_block[16] = {0};
    aes_gcm_calculate_length_block(ctx, length_block);

    /* Iterate through the number of blocks + 1(for the length block) */
    int i, j;

    for (i = 0; i < ctx->num_blocks + 1; i++) {

        /* Pointer to the current block's ciphertext */
        unsigned char *current_ciphertext = ctx->data_encr->ciphertext + (i * 16);

        /* If all the blocks have been processed */
        if (i == ctx->num_blocks) {
            /* Use the length block */
            current_ciphertext = length_block;
        }

        /* XOR the block's ciphertext with the ghash 
        * For the last block, the length block is used.
        */
        for (j = 0; j < 16; j++) {
            ctx->ghash[j] ^= current_ciphertext[j];
        }

        /* Perform GF multiplication: xᵢ = (xᵢ₋₁ ⊕ Cᵢ) • H 
        * Cᵢ = encrypted version of the ith block of plaintext.
        * H = hash subkey.
        */
        gf_multiply_2_128(
            ctx->ghash, ctx->hash_subkey, 16, ctx->ghash
        );
    }
}


void aes_gcm_calculate_tag(aes_gcm_context_t *ctx) {

    /* Calculate the ghash needed for creating the tag */
    aes_gcm_calculate_ghash(ctx);

    /* XOR the GHash with the nonce */
    int i;
    for (i = 0; i < 16; i++) {
        ctx->data_encr->tag[i] = ctx->ghash[i] ^ ctx->y0[i];
    }
}

/* IF debug level 3 is defined*/
#ifdef DEBUG 
void debug_print(aes_gcm_context_t *ctx, int round) {
    /* Variables needed */
    int i, j;

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

    /* If the rounds are over */
    } else if (round == ctx->num_blocks) {

        /* Print the entire ciphertext in 16 block chunks */
        printf("[DEBUG] Final ciphertext : \n");
        for ( i = 0; i <ctx->num_blocks; i++) {
            for (j = 0; j < 16; j++) printf("%02X", ctx->data_encr->ciphertext[i * 16 + j]);
            printf("\n");
        }
    
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
    const unsigned char *nonce) {

    /* Initialize the context needed for AES-GCM encryption */
    aes_gcm_context_t *ctx = aes_gcm_encrypt_init(input, input_size, key, key_size, nonce);

    /* Iterate through all the blocks */
    int i; 
    for (i = 0; i < ctx->num_blocks; i++)
    {
        /* Increment the counter block */
        /* Start at 1 because counter block 0(E(K,Y0)) is reserved for the nonce. */
        aes_gcm_increment_counter_block(ctx);

        /* Generate the keystream block */
        aes_gcm_create_keystream(ctx);

        /* Generate the ciphertext for this block */
        aes_gcm_create_ciphertext(ctx, input + (i * 16));

        /* Debugging if needed */
        #if defined(DEBUG) && DEBUG_LEVEL == 3
            debug_print(ctx, i);
        #endif
    }
    
    /* Calculate the tag */
    aes_gcm_calculate_tag(ctx);

    /* Debugging if needed */
    #if defined(DEBUG) && DEBUG_LEVEL == 3
        debug_print(ctx, i);
    #endif

    /* Extract the encrypted data */
    aes_gcm_encrypted_data_t *data_encr = ctx->data_encr;

    /* Free the memory allocated for the context */
    free(ctx);

    /* Return the encrypted data */
    return data_encr;
}

/*******************************************************************************
 * Encrypts the input using AES-GCM.
 *
 * inputs:
 * - input - The input to encrypt.
 * - input_size - The size of the input.
 * - key - The key to use for the encryption.
 * - key_size - The size of the key.
 *
 * outputs:
 * - None.
 ******************************************************************************/
void aes_encrypt_gcm_bytes(
    const unsigned char *input,
    int input_size,
    const unsigned char *key,
    int key_size) {

    /* Generates a random 12 byte nonce */
    unsigned char *nonce = aes_gcm_generate_nonce();

    /* Encrypt the input */
    aes_encrypt_gcm(input, input_size, key, key_size, nonce);

    /* Free the nonce */
    free(nonce);
}
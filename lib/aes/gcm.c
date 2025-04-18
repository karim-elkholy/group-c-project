#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "portable64/counter.h"
#include "utils/random.h"
#include "aes/core/aes.h"

/* Context needed during GCM encryption */
struct aes_gcm_context
{
    /* The nonce */
    unsigned char *nonce;

    /* The counter block */
    unsigned char *counter_block;

    /* The number of blocks needed */
    int num_blocks;

    /* Holds the input data aka plaintext */
    unsigned char *input;

    /* Holds the input length */
    int input_length;

    /* Holds the bytes processed */
    int bytes_processed;

    /* Holds the ciphertext */
    unsigned char *ciphertext;

    /* Holds the tag */
    unsigned char *tag;

    /* Holds the key */
    const unsigned char *key;

    /* Holds the keystream block */
    unsigned char keystream_block[16];
};
typedef struct aes_gcm_context aes_gcm_context_t;

/*******************************************************************************
 * Checks whether the counter falls under the AES_GCM plaintext limit.
 * The limit is 2^39 - 256 bits which is equal to 68,719,476,704 bytes.
 *
 * Source: NIST SP 800-38D section 5.2.1.1
 *
 * inputs:
 * - counter - The counter to check.
 *
 * outputs:
 * - 1 if the counter is equal to the limit, 0 otherwise.
 */
int is_equal_to_limit(uint64_counter_t *counter)
{
    return (counter->hi == 0x0F && counter->lo == 0xFFFFFFE0);
}

/*******************************************************************************
 * Initializes the counter block for the GCM encryption.
 *
 * inputs:
 * - nonce - The nonce to use for the GCM encryption.
 *
 * outputs:
 * - The counter block.
 ******************************************************************************/
unsigned char *aes_init_counter_block(const unsigned char *nonce)
{
    /* The first counter block is nounce + 4 bytes */
    unsigned char *counter_block = (unsigned char *)malloc(
        16 * sizeof(unsigned char));

    /* Copy the nonce to the counter block */
    memcpy(counter_block, nonce, 12);

    printf("copied nonce to counter block\n");
    exit(0);
    /* For nonces of 12 bytes, the last 4 bytes are 0x00000001 */
    counter_block[12] = 0x00;
    counter_block[13] = 0x00;
    counter_block[14] = 0x00;
    counter_block[15] = 0x01;

#ifdef DEBUG
    /* Convert the counter block to a hex string */
    char *counter_block_hex = convert_bytes_to_hex_string(
        counter_block, 16);

    /* Print the counter block */
    printf("Counter block: %s\n", counter_block_hex);

    /* Free the counter block hex */
    free(counter_block_hex);
#endif

    /* Return the counter block */
    return counter_block;
}

/**
 * Generates a 12-byte random sequence of bytes.
 * This will be the nonce for the GCM encryption.
 *
 * inputs:
 * - None.
 *
 * outputs:
 * - A 12-byte random sequence of bytes.
 */
unsigned char *aes_gcm_generate_nonce()
{
    /* Create a 12-byte random sequence of bytes
     * This will be the nonce for the GCM encryption
     */
    unsigned char *nonce = random_bytes(12);

/* Call the associated debug function */
#ifdef DEBUG

    /* Convert the nonce to a hex string */
    char *nonce_hex = convert_bytes_to_hex_string(nonce, 12);

    /* Print the nonce */
    printf("Nonce: %s\n", nonce_hex);

    /* Free the nonce hex */
    free(nonce_hex);
#endif

    /* Return the nonce */
    return nonce;
}

/*******************************************************************************
 * Initialize the GCM context
 *
 * inputs:
 * - None.
 *
 * outputs:
 * - The GCM context.
 */
aes_gcm_context_t *aes_gcm_init(int input_size)
{
    /* Allocate memory for the context */
    aes_gcm_context_t *context = (aes_gcm_context_t *)malloc(
        sizeof(aes_gcm_context_t));

    /* Initialize the nonce */
    context->nonce = aes_gcm_generate_nonce();

    // print the nonce 
    int i;
    printf("nonce: ");
    for (i = 0; i < 12; i++)
    {
        printf("%02X ", context->nonce[i]);
    }
    printf("\n");
    exit(0);

    printf("created nonce\n");

    /* Initialize the counter block */
    context->counter_block = aes_init_counter_block(context->nonce);

    printf("initialized counter block\n");
    exit(0);
    /* Determine the number of blocks needed
     * Each block will be encrypted in increments of 16 bytes.
     */
    context->num_blocks = (input_size + 15) / 16;

    /* Allocate memory for the ciphertext */
    context->ciphertext = (unsigned char *)malloc(
        input_size * sizeof(unsigned char));

    /* Return the context */
    return context;
}

/*******************************************************************************
 *
 *
 *
 *
 */
/*
void aes_gcm_xor_plaintext_with_keystream(
    const unsigned char *plaintext,
    int plaintext_length,
    const unsigned char *keystream,
) {

}
*/

/*******************************************************************************
 * Generates the ciphertext for the current block. 
 * 
 * inputs:
 * - context - The context to use for the GCM encryption.
 * - plaintext - The plaintext to encrypt.
 * - keystream - The keystream to use for the GCM encryption.
 *
 * outputs:
 * - None.
 ******************************************************************************/
void aes_gcm_create_ciphertext(aes_gcm_context_t *ctx) {
    /* Determine the number of bytes to process */
    int bytes_to_process = ctx->input_length - ctx->bytes_processed;


    /* Iterate through the bytes in this plaintext block */
    int i;
    for (i = 0; i < bytes_to_process; i++)
    {
        /* XOR the plaintext with the keystream */
        ctx->ciphertext[ctx->bytes_processed + i] = ctx->input[i] ^ ctx->keystream_block[i];
    }

    /* Update the bytes processed */
    ctx->bytes_processed += bytes_to_process;
}

void aes_encrypt_gcm_bytes(
    const unsigned char *input,
    int input_size,
    const unsigned char *key,
    int key_size)
{

    /* Initialize the context */
    aes_gcm_context_t *ctx = aes_gcm_init(input_size);

    /* Iterate while there is plaintext to encrypt */
    int i;
    for (i = 0; i < ctx->num_blocks; i++)
    {
        /* Generate the keystream block */
        aes_encrypt(
            input + (i * 16),
            key,
            key_size,
            ctx->keystream_block);

        /* Generate the ciphertext for this block */
        aes_gcm_create_ciphertext(ctx);
    }

    printf("Finished encrypting\n");
    /* Iterate through the ciphertext */
    int j;
    for (j = 0; j < ctx->bytes_processed; j++)
    {
        printf("Ciphertext: %02X\n", ctx->ciphertext[j]);
    }
    
}
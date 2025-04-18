#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "aes/core/aes.h"
#include "aes/core/aes_keyschedule.h"
#include "aes/core/aes_operations.h"

/*******************************************************************************
 * Prints the state matrix.
 * TODO - Remove this function once finished.
 *
 * inputs:
 * - state_matrix - The state matrix to print
 * outputs:
 * - None.
 ******************************************************************************/
void print_state_matrix(byte state[4][4])
{
    int row_idx, col_idx;
    for (row_idx = 0; row_idx < 4; row_idx++)
    {
        for (col_idx = 0; col_idx < 4; col_idx++)
        {
            printf("%02X ", state[row_idx][col_idx]);
        }
        printf("\n");
    }
}

/*******************************************************************************
 * Transforms an array of bytes to a state matrix.
 * The state matrix is stored in column-major order.
 *
 * inputs:
 * - input - The input array of bytes
 * - state - The state matrix to store the result.
 * outputs:
 * - None.
 ******************************************************************************/
void convert_bytes_to_state_matrix(const byte *input, byte state[4][4])
{
    /* Store the matrix in column-major order.
     * Column-major order = Each column stores the next 4 consecutive bytes.
     */
    int row_idx, col_idx;
    for (row_idx = 0; row_idx < 4; row_idx++)
    {
        for (col_idx = 0; col_idx < 4; col_idx++)
        {
            state[row_idx][col_idx] = input[4 * col_idx + row_idx];
        }
    }
}

/*******************************************************************************
 * Converts a state matrix to an array of bytes.
 *
 * inputs:
 * - state - The state matrix to convert
 * - output - The array of bytes to store the result.
 * outputs:
 * - None.
 ******************************************************************************/
void convert_to_byte_array(byte state[4][4], unsigned char *output)
{

    /* Convert the state matrix to a byte array.
     * The state matrix is stored in column-major order.
     * Column-major order = Each column stores the next 4 consecutive bytes.
     */
    int i;
    for (i = 0; i < 16; i++)
    {
        output[i] = state[i % 4][i / 4];
    }
}

/*******************************************************************************
 * AES encryption function.
 *
 * inputs:
 * - input - The input to encrypt. Must be 16 bytes long.
 * - key - The 128, 192, or 256 bit key to use for encryption.
 * - key_size - The size of the key.
 * - output - The output to store the encrypted data.
 * outputs:
 * - None.
 ******************************************************************************/
void aes_encrypt(const byte *input, const byte *key, int key_size, byte *output)
{
    /* Declare variables */
    int i;
    byte input_state[4][4];

    /* Get the keys for each round. */
    roundKeys_t *round_keys = key_expansion(key, key_size);

    /* Convert the input to a state matrix. */
    convert_bytes_to_state_matrix(input, input_state);

    /* Add the initial round key to the input. */
    add_round_key(input_state, round_keys->keys);

    /* Loop based on the number of rounds
     * Number of rounds depends on the key size.
     * 10/12/14 rounds for 128/192/256 bit keys.
     */
    for (i = 1; i < round_keys->count; i++)
    {
        /* SubBytes transformation. */
        sub_bytes(input_state);

        /* ShiftRows transformation. */
        shift_rows(input_state);

        /* If this is not the last round, mix the columns. */
        if (i != round_keys->count - 1)
        {
            /* MixColumns transformation. */
            mix_columns(input_state);
        }

        /* Add the current round key. */
        add_round_key(input_state, round_keys->keys + (i * 16));
    }

    /* Free the round keys. */
    free(round_keys->keys);
    free(round_keys);

    /* Convert the output state matrix to a byte array. */
    convert_to_byte_array(input_state, output);
}

/*******************************************************************************
 * AES decryption function.
 *
 * inputs:
 * - input - The input to decrypt. Must be 16 bytes long.
 * - key - The 128, 192, or 256 bit key to use for decryption.
 * - output - The output to store the decrypted data.
 * outputs:
 * - The decrypted output.
 ******************************************************************************/
/*
byte *aes_decrypt(const byte *input, const byte *key, int key_size, byte *output)
{

}
*/

aes_encrypted_result *aes_encrypt_bytes(const byte *input, int input_size, const byte *key, int key_size)
{
    /* Declare variables */
    aes_encrypted_result *result;
    byte padded_block[16];
    int padding_length;
    int num_blocks;
    byte *output;
    int i;

    /* Assume key size is 128 bits. */
    if (key_size != 16 && key_size != 24 && key_size != 32)
    {
        printf("Key size: %d\n", key_size);
        printf("Invalid key size\n");
        exit(0);
    }

    /* Determine the length of padding needed for the last input block. */
    padding_length = 16 - (input_size % 16);

    /* Split the input into blocks of 16 bytes.
     * An extra block will be needed if the input is a multiple of 16
     */
    num_blocks = input_size / 16 + 1;

    /* Holds the encrypted output. */
    output = (byte *)malloc(num_blocks * 16 * sizeof(byte));

    /* Encrypt the input in 16 byte increments. */
    for (i = 0; i < num_blocks - 1; i++)
    {
        /* Encrypt the input in 16 byte increments. */
        aes_encrypt(input + (i * 16), key, key_size, output + (i * 16));
    }

    /* Create the final block with PKCS#7 padding. */
    memset(padded_block, padding_length, 16);

    /* Copy the remaining data to the padded block. */
    memcpy(padded_block, input + (num_blocks - 1) * 16, input_size % 16);
    aes_encrypt(padded_block, key, key_size, output + ((num_blocks - 1) * 16));

    /* NOTE this uses ECB mode which is different from CBC mode */

    /* Create the result struct. */
    result = (aes_encrypted_result *)malloc(sizeof(aes_encrypted_result));
    result->bytes = output;
    result->size = num_blocks * 16;

    /* Return the encrypted output. */
    return result;
}

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "encryption/aes/core.h"
#include "encryption/aes/keyschedule.h"
#include "encryption/aes/operations.h"
#include "encryption/aes/maths/gf.h"


/*******************************************************************************
 * Transforms an array of bytes to a state matrix.
 * The state matrix is stored in column-major order.
 *
 * inputs:
 * - input - The input array of bytes
 * - state - The state matrix to store the result.
 * outputs:
 * - None.
*******************************************************************************/
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
*******************************************************************************/
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
*******************************************************************************/
void aes_encrypt_block(const byte *input, const byte *key, int key_size, byte *output)
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
 * AES decryption function. TODO
 *
 * inputs:
 * - input - The input to decrypt. Must be 16 bytes long.
 * - key - The 128, 192, or 256 bit key to use for decryption.
 * - output - The output to store the decrypted data.
 * outputs:
 * - None.
*******************************************************************************/
void aes_decrypt_block(const byte *input, const byte *key, int key_size, byte *output)
{
    /* Declare variables */
    int i;
    byte input_state[4][4];

    /* Get the keys for each round. */
    roundKeys_t *round_keys = key_expansion(key, key_size);

    /* Convert the input to a state matrix. */
    convert_bytes_to_state_matrix(input, input_state);

    /* Add the initial round key to the input. */
    /* Use the last round key. */
    add_round_key(input_state, round_keys->keys + ((round_keys->count - 1) * 16));

    /***** Other rounds *****/
    for (i = round_keys->count - 2; i >= 0; i--) {

        /* InvShiftRows transformation. */
        inv_shift_rows(input_state);   

        /* InvSubBytes transformation. */
        inv_sub_bytes(input_state);

        /* Add the current round key to the input. */
        const unsigned char *current_round_key = round_keys->keys + (i * 16);
        add_round_key(input_state, current_round_key);

        /* If this is not the last round, mix the columns. */
        if (i != 0) {
            /* InvMixColumns transformation. */
            inv_mix_columns(input_state);
        }
    }

    /* Free the round keys. */
    free(round_keys->keys);
    free(round_keys);

    /* Convert the output state matrix to a byte array. */
    convert_to_byte_array(input_state, output);
}

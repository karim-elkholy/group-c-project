
#include "encryption/aes/keyschedule.h"
#include "encryption/aes/operations.h"

#include <stdlib.h>
#include <string.h>

/* Context needed during key expansion */
struct key_expansion_context
{
    /* Transformation frequency
     * Every nth word undergoes a special process
     */
    int num_words;

    /* Key size
     * The size of the key.
     * 128 bits/16 bytes = AES-128 key
     * 192 bits/24 bytes = AES-192 key
     * 256 bits/32 bytes = AES-256 key
     */
    int key_size;

    /* Number of rounds to generate */
    int num_rounds;

    /* Total number of words across all the round keys */
    int total_words;

    /* Keeps track of the number of words processed */
    int words_processed;

    /* Array of round keys */
    roundKeys_t *round_keys;
};
typedef struct key_expansion_context key_expansion_context_t;

/*******************************************************************************
 * Get the number of words in each round key.
 * A word is defined as 32 bits or 4 bytes.
 *
 * The number of words in each round key is determined by the key size.
 * 128-bit/16-byte key = 4 words
 * 192-bit/24-byte key = 6 words
 * 256-bit/32-byte key = 8 words
 *
 * Number of words can manually be calculated using:
 *  1. (key_size_bits / 32)
 *  2. (key_size_bytes / 4)
 *
 * inputs:
 * - key_size - The size of the key
 * outputs:
 * - The number of words in each round key.
 ******************************************************************************/
int keyschedule_get_number_of_words(int key_size)
{
    /* Return the number of words in the key.
     * 128-bit key = 4 words
     * 192-bit key = 6 words
     * 256-bit key = 8 words
     */
    return key_size / 4;
}

/*******************************************************************************
 * Determine the number of rounds.
 *
 * The number of rounds is determined by the key size.
 *
 * 128-bit key = 10 rounds
 * 192-bit key = 12 rounds
 * 256-bit key = 14 rounds
 *
 * Number of rounds can manually be calculated using:
 *  1. (key_size_bits / 32) + 6
 *  2. (key_size_bytes / 4) + 6
 *
 * inputs:
 * - key_size - The size of the key
 * outputs:
 * - The number of rounds.
 ******************************************************************************/
int keyschedule_get_number_of_rounds(int key_size)
{
    /* Return the number of rounds. */
    return (key_size / 4) + 6;
}

/**
 * Initialize the key expansion context.
 *
 * inputs:
 * - key - The key to use for the key expansion.
 * - key_size - The size of the key.
 * outputs:
 * - The key expansion context.
 */
key_expansion_context_t *key_expansion_init(const unsigned char *key, int key_size)
{
    /* Allocate memory for the context */
    key_expansion_context_t *context = malloc(sizeof(key_expansion_context_t));

    /* Set the key size */
    context->key_size = key_size;

    /* Determine how many words are required for each round key.
     * This differs based on the key size.
     */
    context->num_words = keyschedule_get_number_of_words(key_size);

    /* Determine the number of round keys to generate.
     * This differs based on the key size.
     */
    context->num_rounds = keyschedule_get_number_of_rounds(key_size);

    /* Calculate the total number of words across all the round keys.
     * A word is 4 bytes.
     * Round keys will have 4 words since a round key is always 16 bytes.
     */
    context->total_words = (context->num_rounds + 1) * 4;

    /* Allocate memory for the round keys. */
    context->round_keys = malloc(sizeof(roundKeys_t));
    context->round_keys->keys = malloc((context->num_rounds + 1) * 16);
    context->round_keys->count = context->num_rounds + 1;

    /* Copy the original key to the start of the round keys array. */
    memcpy(context->round_keys->keys, key, key_size);

    /* Update the number of words processed */
    context->words_processed = context->num_words;

    /* Return the context */
    return context;
}

/*******************************************************************************
 * Special process for each nth word that transforms the word.
 * n = 4/6/8 for 128/192/256-bit keys
 *
 * Transformation consists of the following steps:
 * 1. Rotate the last word to the left by 1 byte.
 * 2. Substitute the bytes with their value from the S-box.
 * 3. XOR the word with the Rcon value for the current round.
 *
 * inputs:
 * - context - The context.
 * - word - The word to transform.
 * outputs:
 * - none
 */
void key_expansion_transform(key_expansion_context_t *context, unsigned char *word)
{
    /* Copy the previous word into the current word
     * This is the first step to generate the actual current word
     */
    /* memcpy(word, word - 4, 4); */

    /* Rotate the last word to the left by 1 byte. */
    rotate_word(word);

    /* Substitute the bytes with their value from the S-box. */
    sub_word(word);

    /* XOR the word with the Rcon value for the current round.
     * Only the first byte of the word is XORed with the Rcon value.
     */
    word[0] ^= rcon_table[context->words_processed / context->num_words];
}

/**
 * Extra step needed when handling AES-256 keys.
 * TODO - Describe this step better.
 * 
 * inputs:
 * - context - The context.
 * - word - The current word.
 * outputs:
 * - none
 */
void key_expansion_handle_256_key(
    key_expansion_context_t *context, 
    unsigned char *word)
{
    /* Every 4th word undergoes a special process */
    if (context->words_processed % context->num_words == 4)
    {
        /* Substitute the bytes with their value from the S-box. */
        sub_word(word);

        /* Perform the XOR operation needed by the keySchedule */
        /* key_expansion_xor_transform( context,  copy, word); */
    }
}

unsigned char *key_expansion_get_current_word(key_expansion_context_t *context)
{
    return context->round_keys->keys + (context->words_processed * 4);
}

/**
 * XOR the current word with the word 'n' words before it.
 * n = 4/6/8 depending on the key size.
 * This is the final step of key expansion for each word.
 *
 * @param context The context.
 * @param current_word The current word.
 */
void key_expansion_xor_word(
    key_expansion_context_t *context,
    unsigned char *current_word)
{
    /* Get the word n words before the current word
     * n = 4/6/8 depending on the key size
     * This is the word that will be XORed with the current word
     */
    unsigned char *prev_word_n = current_word - (4 * context->num_words);

    /* XOR the current word with the word n words before it */
    int i;
    for (i = 0; i < 4; i++)
    {
        current_word[i] ^= prev_word_n[i];
    }
}

void key_expansion_expand_keys(
    key_expansion_context_t *context,
    const unsigned char *key)
{
    /* Iterate until there are no more words are left */
    while (context->words_processed < context->total_words)
    {
        /* Get the current word */
        unsigned char *current_word = key_expansion_get_current_word(context);

        /* Copy the previous word into the current word
         * This is the first step to generate the actual current word
         */
        memcpy(current_word, current_word - 4, 4);

        /* Every nth word needs special treatment
         * n = 4/6/8 for 128/192/256-bit keys
         */
        if (context->words_processed % context->num_words == 0)
        {
            key_expansion_transform(context, current_word);

            /* Other words are XORed with the word n words before it */
        }
        else
        {
            /* Special handling required for AES-256 keys */
            if (context->key_size == 32)
            {
                key_expansion_handle_256_key(context, current_word);
            } 
        }

        /* Final step of key expansion for the current word */
        key_expansion_xor_word(context, current_word);

        /* Another word has been processed */
        context->words_processed += 1;
    }
}

/**
 * Expand the key into a list of round keys.
 *
 * @param key The key to expand.
 * @param key_size The size of the key.
 * @return The expanded key.
 */
roundKeys_t *key_expansion(const unsigned char *key, int key_size)
{
    /* Initialize the context */
    key_expansion_context_t *context = key_expansion_init(key, key_size);

    /* Generate the round keys */
    key_expansion_expand_keys(context, key);

    /* Store the round keys */
    roundKeys_t *round_keys = context->round_keys;

    /* Free the context since round key generation is complete*/
    free(context);

    /* Return the round keys */
    return round_keys;
}

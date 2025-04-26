#include "utils/hash.h"
/*******************************************************************************
 * Hashes the given text using XOR.
 * 
 * inputs:
 * - text - The text to hash
 * outputs:
 * - The hashed text
 ******************************************************************************/
unsigned int hash_text(const char *text) {
    /* Hash the text */
    unsigned int hash = 0;

    /* XOR each character with a key */
	int i;
    for (i = 0; i < strlen(text); i++) {
        hash ^= text[i];
    }
    return hash;
}
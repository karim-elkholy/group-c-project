#include "utils/hash.h"
#include <string.h>
/*******************************************************************************
 * Hashes the given string using XOR.
 * 
 * inputs:
 * - string - The string to hash
 * outputs:
 * - The hashed string
 ******************************************************************************/
unsigned int hash_string(const char *string) {
    /* Hash the string */
    unsigned int hash = 0;

    /* XOR each character with a key */
	int i;
    for (i = 0; i < strlen(string); i++) {
        hash ^= string[i];
    }
    return hash;
}
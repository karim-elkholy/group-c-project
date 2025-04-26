#ifndef UTILS_HASH_H
#define UTILS_HASH_H

/*******************************************************************************
 * Hashes the given text using XOR.
 * 
 * inputs:
 * - text - The text to hash
 * outputs:
 * - The hashed text
 ******************************************************************************/
unsigned int hash_text(const char *text);

#endif


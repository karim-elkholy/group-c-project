#ifndef AES_KEYSCHEDULE_H
#define AES_KEYSCHEDULE_H

/*
 * Holds the round keys generated during the keySchedule process.
 */
struct roundKeys
{
    /* Array of round keys. */
    unsigned char *keys;

    /* Number of keys present */
    int count;
};
typedef struct roundKeys roundKeys_t;

/* KeySchedule functions */
roundKeys_t *key_expansion(const unsigned char *key, int key_size);

#endif
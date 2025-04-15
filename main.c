
#include "aes.h"
#include <stdio.h>
#include <stdlib.h> // For malloc
#include <string.h> // For strlen

int main()
{
    // Input: "00112233445566778899aabbccddeeff" (16 bytes)
    const byte input_bytes[] = {
        0x00, 0x11, 0x22, 0x33,
        0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb,
        0xcc, 0xdd, 0xee, 0xff};

    // Key: "000102030405060708090a0b0c0d0e0f" (16 bytes)
    const byte key_bytes_16[] = {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f};


    // Key: "000102030405060708090a0b0c0d0e0f1011121314151617" (24 bytes)
    const byte key_bytes_24[] = {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17};


    aes_encrypted_result *result = aes_encrypt_bytes(input_bytes, 16, key_bytes_16, 16);

    // Print the encrypted blocks.
    printf("Encrypted Message: ");
    int i;
    for ( i = 0; i < result->size; i++ )
    {
        printf("%02X ", result->bytes[i]);
    }
    printf("\n");

    /* Free the result once finished */
    free(result->bytes);
    free(result);
}


#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

#include "aes/core/aes.h"
#include "aes/gcm.h"
#include "utils/hex.h"
#include "aes/core/aes_operations.h"

void test_bytes_to_hex_str() {
    
    const byte input_bytes[] = {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f};

    const char *expected_hex = "000102030405060708090A0B0C0D0E0F";
    char *actual_hex = convert_bytes_to_hex_string(input_bytes, 16);

    if (strcmp(actual_hex, expected_hex) != 0) {
        printf("Test failed\n");
        printf("Expected: %s\n", expected_hex);
        printf("Actual: %s\n", actual_hex);
        exit(1);
    }

    free(actual_hex);
}

void test_hex_str_to_bytes() {
    int i;
    const char *input_hex = "000102030405060708090A0B0C0D0E0F";
    byte *actual_bytes = convert_hex_string_to_bytes(input_hex);

    const byte expected_bytes[] = {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f};

    if (memcmp(actual_bytes, expected_bytes, 16) != 0) {
        printf("Test failed\n");

        /* Print the expected and actual bytes */
        printf("Expected: ");
        for (i = 0; i < 16; i++) {
            printf("%02X ", expected_bytes[i]);
        }
        printf("\n");
        printf("Actual: ");
        for (i = 0; i < 16; i++) {
            printf("%02X ", actual_bytes[i]);
        }
        exit(1);
    }

    free(actual_bytes);
}


void test_run_method( const char *test_name, void (*test_method)()) {
    printf("Running test: %s\n", test_name);
    test_method();
    printf("Test passed [+]\n");
}

/* Validates AES implementation
 * Compares outputs against official examples provided in FIPS-197 standard.
 * 
 * FIPS-197 Appendix C - Example Vectors 
 * https://csrc.nist.gov/files/pubs/fips/197/final/docs/fips-197.pdf
*/
void test_fips_example() {

    /* Declare variables */
    int i;
    byte *keys[3];    
    const char *expected_hex[3];
    int key_sizes[3];

    /* Input: "00112233445566778899aabbccddeeff" (16 bytes) */
    byte *input_bytes = convert_hex_string_to_bytes("00112233445566778899aabbccddeeff");

    /* Key: "000102030405060708090a0b0c0d0e0f" (16 bytes) */
    byte *key_bytes_16 = convert_hex_string_to_bytes("000102030405060708090a0b0c0d0e0f");
    const char *expected_hex_16 = "69C4E0D86A7B0430D8CDB78070B4C55A";
    /* Key: "000102030405060708090a0b0c0d0e0f1011121314151617" (24 bytes) */
    byte *key_bytes_24 = convert_hex_string_to_bytes("000102030405060708090a0b0c0d0e0f1011121314151617");
    const char *expected_hex_24 = "DDA97CA4864CDFE06EAF70A0EC0D7191";
    /* Key: "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f" (32 bytes) */
    byte *key_bytes_32 = convert_hex_string_to_bytes("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
    const char *expected_hex_32 = "8EA2B7CA516745BFEAFC49904B496089";

    /* Store the different keys */
    keys[0] = key_bytes_16;
    keys[1] = key_bytes_24;
    keys[2] = key_bytes_32;
    
    /* Store the expected hex strings */
    expected_hex[0] = expected_hex_16;
    expected_hex[1] = expected_hex_24;
    expected_hex[2] = expected_hex_32;

    /* Store the key sizes */
    key_sizes[0] = 16;
    key_sizes[1] = 24;
    key_sizes[2] = 32;

    /* Iterate over the different keys */
    for (i = 0; i < 3; i++) {

        /* Expected output: "69c4e0d86a7b0430d8cdb78070b4c55a" */
        aes_encrypted_result *result = aes_encrypt_bytes(input_bytes, 16, keys[i], key_sizes[i]);

        /* Convert the result to a hex string */
        char *result_hex = convert_bytes_to_hex_string(result->bytes, result->size);

        /* If the result starts with the expected hex 
         * Consider the test passed 
         * Because the FIPS-197 examples do not include the padding 
         */
        if (strncmp(result_hex, expected_hex[i], strlen(expected_hex[i])) != 0) {
            printf("Test failed\n");
            printf("Key size: %d\n", key_sizes[i]);
            printf("Expected: %s\n", expected_hex[i]);
            printf("Actual: %s\n", result_hex);
            exit(1);
        }

        /* Free the result & the current key */
        free(keys[i]);
        free(result->bytes);
        free(result);
        free(result_hex);
    }

    /* Free the input bytes */
    free(input_bytes);
}

void test_aes_128_encrypt() {

    /* 16 byte message */
    /* Plaintext: "Turtles are cool" */
    const byte *msg = (byte *)"Turtles are cool";

    /* 16 byte key */
    /* Key: "Cats are weirdos" */
    const byte *key = (byte *)"Cats are weirdos";

    /* Expected output */
    const char *expected_output = "24D81B412E3B205A8D4D202DB5472A18C0E0CA118F2D9E068E76CE2888D836B0";

    /* Encrypt the message */
    /* Retrieved from https://www.devglan.com/online-tools/aes-encryption-decryption */
    aes_encrypted_result *result = aes_encrypt_bytes(msg, 16, key, 16);

    /* Convert the result to a hex string */
    char *result_hex = convert_bytes_to_hex_string(result->bytes, result->size);

    /* Compare the actual output with the expected output */
    if (strcmp(result_hex, expected_output) != 0) {
        printf("Test failed\n");
        printf("Expected: %s\n", expected_output);
        printf("Actual: %s\n", result_hex);
        exit(1);
    }

    /* Free the result */
    free(result->bytes);
    free(result);
    free(result_hex);
}

void test_aes_192_encrypt() {

    /* 16 byte message */
    /* Plaintext: "Turtles are cool" */
    const byte *msg = (byte *)"Turtles are cool";

    /* 32 byte key */
    /* Key: "32 characters is more than 24 :(" */
    const byte *key = (byte *)"32 characters is more than 24 :(";

    /* Expected output */
    /* Retrieved from https://www.devglan.com/online-tools/aes-encryption-decryption */
    const char *expected_output = "257A346376C0B039EBCAFAFB441AC8ABD297BCE2FF660D228281608815094850";

    /* Encrypt the message */
    aes_encrypted_result *result = aes_encrypt_bytes(msg, 16, key, 32);

    /* Convert the result to a hex string */
    char *result_hex = convert_bytes_to_hex_string(result->bytes, result->size);

    /* Compare the actual output with the expected output */
    if (strcmp(result_hex, expected_output) != 0) {
        printf("Test failed\n");
        printf("Expected: %s\n", expected_output);
        printf("Actual: %s\n", result_hex);
        exit(1);
    }

    /* Free the result */
    free(result->bytes);
    free(result);
    free(result_hex);
}

void test_aes_256_encrypt() {

    /* 16 byte message */
    /* Plaintext: "Turtles are cool" */
    const byte *msg = (byte *)"Turtles are cool";

    /* 24 byte key */
    /* Key: "24 characters is a lot ." */
    const byte *key = (byte *)"24 characters is a lot .";

    /* Expected output */
    /* Retrieved from https://www.devglan.com/online-tools/aes-encryption-decryption */
    const char *expected_output = "37FF5F80C1F7A20579A4C825AC8AE5B7FED03DBCDD212C3D36339D0DA613D8C7";

    /* Encrypt the message */
    aes_encrypted_result *result = aes_encrypt_bytes(msg, 16, key, 24);

    /* Convert the result to a hex string */
    char *result_hex = convert_bytes_to_hex_string(result->bytes, result->size);

    /* Compare the actual output with the expected output */
    if (strcmp(result_hex, expected_output) != 0) {
        printf("Test failed\n");
        printf("Expected: %s\n", expected_output);
        printf("Actual: %s\n", result_hex);
        exit(1);
    }

    /* Free the result */
    free(result->bytes);
    free(result);
    free(result_hex);
}

/* Taken from https://csrc.nist.rip/groups/ST/toolkit/BCM/documents/proposedmodes/gcm/gcm-spec.pdf*/
void test_aes_gcm_encrypt_case_2() {
    /* 16 byte message */
    /* All zeros */
    const unsigned char plaintext[16] = {0};
    
    /* 16 byte key */
    /* All zeros */
    const unsigned char key[16] = {0};

    /* Nonce to use for encryption */
    const unsigned char nonce[12] = {0};

    /* Encrypt the message */
    aes_encrypt_gcm(plaintext, 16, key, 16, nonce);
}

/* Taken from https://csrc.nist.rip/groups/ST/toolkit/BCM/documents/proposedmodes/gcm/gcm-spec.pdf*/
void test_aes_gcm_encrypt_case_3() {
    /* 60 byte message */
    const unsigned char plaintext[64] = {
        0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
        0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
        0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
        0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39, 0x1a, 0xaf, 0xd2, 0x55
    };
    
    /* 16 byte key */
    const unsigned char key[16] = {
        0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
        0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08
    };
    
    /* 12 byte nonce */
    const unsigned char nonce[12] = {
        0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad,
        0xde, 0xca, 0xf8, 0x88
    };
    
    /* Encrypt the message */
    aes_encrypt_gcm(plaintext, 64, key, 16, nonce);
    
    
}

int main() {

    #ifdef DEBUG
        printf("DEBUG is defined\n");
    #else
        printf("DEBUG is not defined\n");
    #endif

    /* Test AES-GCM */

    test_run_method("convert bytes to hex string", test_bytes_to_hex_str);
    test_run_method("convert hex string to bytes", test_hex_str_to_bytes);
    test_run_method("FIPS examples", test_fips_example);
    test_run_method("AES 128 encrypt", test_aes_128_encrypt);
    test_run_method("AES 192 encrypt", test_aes_192_encrypt);
    test_run_method("AES 256 encrypt", test_aes_256_encrypt);
    test_run_method("AES-GCM encrypt case 2", test_aes_gcm_encrypt_case_3); 
    test_run_method("AES-GCM encrypt case 3", test_aes_gcm_encrypt_case_3);


    return 0;
}
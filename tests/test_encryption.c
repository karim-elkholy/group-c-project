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

void test_aes_gcm_all() {

    /* Ignore this it is being worked on */
    
    /* Number of test cases */
    const int num_test_cases = 7;

    /* Allocate memory to hold the details of each test case */
    /* Plaintext */
    unsigned char **plaintexts = malloc(num_test_cases * sizeof(unsigned char *));
    /* Plaintext size */
    int *plaintext_sizes = malloc(num_test_cases * sizeof(int));
    /* Key */
    unsigned char **keys = malloc(num_test_cases * sizeof(unsigned char *));
    /* Key size */
    int *key_sizes = malloc(num_test_cases * sizeof(int));
    /* Nonce */
    unsigned char **nonces = malloc(num_test_cases * sizeof(unsigned char *));
    /* Nonce size */
    int *nonce_sizes = malloc(num_test_cases * sizeof(int));
    /* AAD(Additional Authentication Data)
    * Data that is not encrypted but is authenticated */
    unsigned char **aads = malloc(num_test_cases * sizeof(unsigned char *));
    /* AAD size*/
    int *aad_sizes = malloc(num_test_cases * sizeof(int));
    /* Expected ciphertext */
    unsigned char **expected_ciphertexts = malloc(num_test_cases * sizeof(unsigned char *));
    /* Expected authentication tags */
    unsigned char **expected_tags = malloc(num_test_cases * sizeof(unsigned char *));

    /* Test case 1
    * The plaintext is actually empty. This is not a mistake or typo.
    * This is the first test case in the NIST document.
    */
    plaintexts[0] = convert_hex_string_to_bytes("");
    plaintext_sizes[0] = 0;
    keys[0] = convert_hex_string_to_bytes("00000000000000000000000000000000");
    key_sizes[0] = 16;
    nonces[0] = convert_hex_string_to_bytes("000000000000000000000000");
    nonce_sizes[0] = 12;
    aads[0] = convert_hex_string_to_bytes("");
    aad_sizes[0] = 0;
    expected_ciphertexts[0] = convert_hex_string_to_bytes("");
    expected_tags[0] = convert_hex_string_to_bytes("58e2fccefa7e3061367f1d57a4e7455a");

    /* Test case 2 */
    plaintexts[1] = convert_hex_string_to_bytes("00000000000000000000000000000000");
    plaintext_sizes[1] = 16;
    keys[1] = convert_hex_string_to_bytes("00000000000000000000000000000000");
    key_sizes[1] = 16;
    nonces[1] = convert_hex_string_to_bytes("000000000000000000000000");
    nonce_sizes[1] = 12;
    aads[1] = convert_hex_string_to_bytes("");
    aad_sizes[1] = 0;
    expected_ciphertexts[1] = convert_hex_string_to_bytes("0388dace60b6a392f328c2b971b2fe78");
    expected_tags[1] = convert_hex_string_to_bytes("ab6e47d42cec13bdf53a67b21257bddf");

    /* Test case 3 */
    plaintexts[2] = convert_hex_string_to_bytes("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255");
    plaintext_sizes[2] = 64;


    keys[2] = convert_hex_string_to_bytes("feffe9928665731c6d6a8f9467308308");
    key_sizes[2] = 16;
    nonces[2] = convert_hex_string_to_bytes("cafebabefacedbaddecaf888");
    nonce_sizes[2] = 12;
    aads[2] = convert_hex_string_to_bytes("");
    aad_sizes[2] = 0;
    expected_ciphertexts[2] = convert_hex_string_to_bytes("42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985");
    expected_tags[2] = convert_hex_string_to_bytes("4d5c2af327cd64a62cf35abd2ba6fab4");

    /* Test case 4 */
    plaintexts[3] = convert_hex_string_to_bytes("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39");
    plaintext_sizes[3] = 60;
    keys[3] = convert_hex_string_to_bytes("feffe9928665731c6d6a8f9467308308");
    key_sizes[3] = 16;
    nonces[3] = convert_hex_string_to_bytes("cafebabefacedbaddecaf888");
    nonce_sizes[3] = 12;
    aads[3] = convert_hex_string_to_bytes("feedfacedeadbeeffeedfacedeadbeefabaddad2");
    aad_sizes[3] = 20;
    expected_ciphertexts[3] = convert_hex_string_to_bytes("42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091");
    expected_tags[3] = convert_hex_string_to_bytes("5bc94fbc3221a5db94fae95ae7121a47");

    /* Test case 5 */
    plaintexts[4] = convert_hex_string_to_bytes("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39");
    plaintext_sizes[4] = 60;
    keys[4] = convert_hex_string_to_bytes("feffe9928665731c6d6a8f9467308308");
    key_sizes[4] = 16;
    nonces[4] = convert_hex_string_to_bytes("cafebabefacedbad");
    nonce_sizes[4] = 8;
    aads[4] = convert_hex_string_to_bytes("feedfacedeadbeeffeedfacedeadbeefabaddad2");
    aad_sizes[4] = 20;
    expected_ciphertexts[4] = convert_hex_string_to_bytes("61353b4c2806934a777ff51fa22a4755699b2a714fcdc6f83766e5f97b6c742373806900e49f24b22b097544d4896b424989b5e1ebac0f07c23f4598");
    expected_tags[4] = convert_hex_string_to_bytes("3612d2e79e3b0785561be14aaca2fccb");

    /* Test case 6 */
    plaintexts[5] = convert_hex_string_to_bytes("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39");
    plaintext_sizes[5] = 60;
    keys[5] = convert_hex_string_to_bytes("feffe9928665731c6d6a8f9467308308");
    key_sizes[5] = 16;
    nonces[5] = convert_hex_string_to_bytes("9313225df88406e555909c5aff5269aa6a7a9538534f7da1e4c303d2a318a728c3c0c95156809539fcf0e2429a6b525416aedbf5a0de6a57a637b39b");
    nonce_sizes[5] = 60;
    aads[5] = convert_hex_string_to_bytes("feedfacedeadbeeffeedfacedeadbeefabaddad2");
    aad_sizes[5] = 20;
    expected_ciphertexts[5] = convert_hex_string_to_bytes("8ce24998625615b603a033aca13fb894be9112a5c3a211a8ba262a3cca7e2ca701e4a9a4fba43c90ccdcb281d48c7c6fd62875d2aca417034c34aee5");
    expected_tags[5] = convert_hex_string_to_bytes("619cc5aefffe0bfa462af43c1699d050");

    /* Test case 7 */
    plaintexts[6] = convert_hex_string_to_bytes("");
    plaintext_sizes[6] = 0;
    keys[6] = convert_hex_string_to_bytes("000000000000000000000000000000000000000000000000");
    key_sizes[6] = 24;
    nonces[6] = convert_hex_string_to_bytes("000000000000000000000000");
    nonce_sizes[6] = 12;
    aads[6] = convert_hex_string_to_bytes("");
    aad_sizes[6] = 0;
    expected_ciphertexts[6] = convert_hex_string_to_bytes("");
    expected_tags[6] = convert_hex_string_to_bytes("cd33b28ac773f74ba00ed1f312572435");

    /* TODO:
    * Skip rest of cases for now since they deal with keys greater than 16 bytes */

    /* Iterate over the test cases */
    int i;
    for (i = 0; i < num_test_cases; i++) {

        /* Print the test case number */
        printf("Test case %d\n", i + 1);

        /* If the key size is not 16, skip the test case */
        if (key_sizes[i] != 16) {
            continue;
        }

        /* If the nonce is not 12 bytes, skip the test case */
        if (nonce_sizes[i] != 12) {
            continue;
        }

        /* Encrypt the plaintext */
        aes_gcm_encrypted_data_t *enc_data = aes_encrypt_gcm(
            plaintexts[i], plaintext_sizes[i],
            keys[i], 16,
            aads[i], aad_sizes[i],
            nonces[i]
        );

        /* Compare the ciphertexts */
        if ( memcmp(enc_data->ciphertext, expected_ciphertexts[i], enc_data->ciphertext_length) != 0 ) {
            printf("Test failed\n");
            printf("Ciphertexts do not match\n");
            printf("Expected: %s\n", convert_bytes_to_hex_string(
                expected_ciphertexts[i], 
                enc_data->ciphertext_length)
            );
            printf("Actual: %s\n", convert_bytes_to_hex_string(
                enc_data->ciphertext, 
                enc_data->ciphertext_length)
            );
            exit(1);
        }

        
        /* Compare the tag */
        if (memcmp(enc_data->tag, expected_tags[i], 16) != 0) {
            printf("Test failed\n");
            printf("Authentication tags do not match\n");
            printf("Expected: %s\n", convert_bytes_to_hex_string(expected_tags[i], 16));
            printf("Actual: %s\n", convert_bytes_to_hex_string(enc_data->tag, 16));
            exit(1);
        }


    }
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
    test_run_method("AES-GCM all", test_aes_gcm_all);

    exit(0);

    return 0;
}
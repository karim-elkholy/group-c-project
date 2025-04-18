#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Generates n random bytes 
 * 
 * inputs:
 * - n - The number of bytes to generate.
 * 
 * outputs:
 * - The random bytes.
 ******************************************************************************/

/* todo - ask teacher whether this will cause style issues */
unsigned char *random_bytes(int n) {

    /* Declare variables needed */
    /* todo - ask teacher whether this will cause style issues */
    unsigned char *random_bytes;
    FILE *fp;
    size_t bytes_read;

    #ifdef DEBUG
    printf("[DEBUG] Generating %d random bytes\n", n);
    #endif

    /* If n is less than 1, return NULL */
    if (n < 1) {
        #ifdef DEBUG
        printf("[DEBUG] Error: 'n' must be greater than 0\n");
        #endif
        return NULL;
    }

    /* Open the /dev/urandom file */
    fp = fopen("/dev/urandom", "rb");

    /* Print an error if the file could not be opened */
    if (fp == NULL) {
        #ifdef DEBUG
        printf("[DEBUG] Error: could not open /dev/urandom\n");
        #endif
        return NULL;
    }

    /* Allocate memory for the random bytes */
    random_bytes = (unsigned char *) malloc(
        n * sizeof(unsigned char)
    );

    #ifdef DEBUG
    printf("[DEBUG] Memory allocated for %d bytes\n", n);
    #endif

    /* Read the random bytes */
    bytes_read = fread(random_bytes, 1, n, fp);

    #ifdef DEBUG
    printf("[DEBUG] Read %lld bytes\n", bytes_read);
    #endif

    /* Print an error if the read was incomplete */
    if (bytes_read != n) {
        #ifdef DEBUG
        printf("[DEBUG] Error: incomplete read \n");
        printf("[DEBUG] Expected %d bytes, read %lld bytes\n", n, bytes_read);
        #endif

        free(random_bytes);
        return NULL;
    }

    /* Close the file */
    fclose(fp);

    /* Return the random bytes */
    return random_bytes;
}


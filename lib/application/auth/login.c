#include "utils/hash.h"
#include "utils/scanner.h"

/*******************************************************************************
 * Verifies the user's password.
 * 
 * inputs:
 * - user - The user to verify
 * outputs:
 * - 1 if the password is correct, otherwise 0
 ******************************************************************************/
int verify_user_password(unsigned int expected_pass_hash) {

    /* Counter for keeping track of the number of login attempts */
    int counter = 3;

    while ( counter != 0 ) {

        /* Store the user's password */
        char user_password[256];
        readString("Enter your password: ", user_password, sizeof(user_password));

        /* Consider the user verified if the password is correct */
        if (expected_pass_hash == hash_text(user_password) ) {
            return 1;
        }

        /* Increment the counter each time an incorrect password is entered */
        counter -= 1;
        printf("Incorrect password, %d attempts remaining\n", counter);
    }

    /* False if the password could not be verified.*/
    return 0;
}

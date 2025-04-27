#include "utils/scanner.h"
#include "utils/validation.h"

#include <string.h>

/*******************************************************************************
 * Asks for a name-like input and ensures it is valid.
 * 
 * inputs:
 * - prompt - The prompt to display to the user.
 * - name - Stores the given name.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_name(const char *prompt, char *name) {

    /* Loop until a valid name is entered */
    while ( 1 ) {

        /* Ask for the name */
        readString(prompt, name, sizeof(name));

        /* If the name is too short */
        if (strlen(name) < 2) {
            printf("Name must be at least 2 characters long\n");
            continue;
        }

        /* Ensure the name only contains letters */
        if (!is_only_letters(name)) {
            printf("Name must only contain letters\n");
            continue;
        }

        /* Once the name is valid, break the loop */
        break;
    }
}

/*******************************************************************************
 * Asks for a password and confirms it.
 * 
 * inputs:
 * - password - Stores the given password.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_password(char *password) {

    /* Used to confirm the password */
    char confirm_password[256];

    /* Ask for the password and confirm it */
    /* By default, assume the passwords do not match */
    int password_match = -1;

    do {
        readString("Enter your password: ", password, sizeof(password));
        readString("Confirm your password: ", confirm_password, sizeof(confirm_password));

        /* Check if the passwords match */
        int password_match = strcmp(password, confirm_password);
        if (password_match != 0) { printf("Passwords do not match\n"); }

    } while (password_match != 0);
}

/*******************************************************************************
 * Asks for an email.
 * 
 * inputs:
 * - email - Stores the given email.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_email(char *email) {

    /* Loop until a valid email is entered */
    while (1) {

        /* Ask for the email */
        readString("Enter your email: ", email, sizeof(email));

        /* If the email is not valid, print an error message */
        if (!is_valid_email(email)) {
            printf("Invalid email\n");
            continue;
        }

        /* Once the email is valid, break the loop */
        break;
    }
}

/*******************************************************************************
 * Asks for a phone number. 
 * A number will only be considered valid if it meets the following conditions:
 * - It is only numbers
 * - It is 10 digits long
 * 
 * inputs:
 * - phone - Stores the given phone number.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_phone(char *phone) {

    /* Loop until a valid phone number is entered */
    while (1) {

        /* Ask for the phone number */
        readString("Enter your phone number: ", phone, sizeof(phone));

        /* If the phone number is not 10 digits long, print an error message */
        if (strlen(phone) != 10) {
            printf("Phone number must be 10 digits long\n");
            continue;
        }

        /* If the phone number is not valid, print an error message */
        if (!is_only_numbers(phone)) {
            printf("Phone number must only contain numbers\n");
            continue;
        }

        /* Once the phone number is valid, break the loop */
        break;
    }
}



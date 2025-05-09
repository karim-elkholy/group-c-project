#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Validates whether the given character is a digit.
 *
 * inputs:
 * - character - The character to check.
 * outputs:
 * - 1 if the character is a digit otherwise 0.
 *******************************************************************************/
int isDigit(char character)
{
	/* Checks if a character is a digit between 0 and 9
	 * '0' - '9' will fall within the range of 48-57 in decimal form
	 * 48 is the ASCII value of '0' and 57 is the ASCII value of '9' */
	return character >= '0' && character <= '9';
}

/*******************************************************************************
 * Validates whether the given string is all digits.
 *
 * inputs:
 * - string - The string to validate
 * outputs:
 * - 1 if the string is all digits otherwise 0.
 *******************************************************************************/
int isAllDigits(const char *string)
{
	/* Iterate through the given string */
	int i;
	for (i = 0; i < strlen(string); i++)
	{
		/* Ensure each character is a number between 0 and 9 */
		if (isDigit(string[i]) == 0)
		{
			/* 0 will be returned if the character is not a number */
			return 0;
		}
	}

	/* 1 will be returned since no issues were found */
	return 1;
}

/*******************************************************************************
 * Validates whether the given email is valid.
 *
 * inputs:
 * - email - The email to check.
 * outputs:
 * - 1 if the email is valid otherwise 0.
 *******************************************************************************/
int is_valid_email(const char *email) {
    /* Check if the email is valid */
    if (email == NULL) {
        return 0;
    }

    /* Create a working copy of the email */
    char *working_email = (char *) malloc(strlen(email) * sizeof(char));
    strcpy(working_email, email);
    
    /* Holds whether the email is valid */
    /* By default, assume the email is invalid */
    int is_email = 0;

    /* Split the email into local part and domain part */
    char *local_part = strtok(working_email, "@");
    char *domain_part = strtok(NULL, "@");

    /* Check if the local part and domain part are valid */
    if (local_part != NULL && domain_part == NULL) {

        /* If domain does not contain a dot, return 0 */
        if (strchr(domain_part, '.') == NULL) {
            /* The email should be considered valid */
            is_email = 1;
        }
    }

    /* Free the working copy of the email */
    free(working_email);

    /* Return whether the email is valid */
    return is_email;
}

/*******************************************************************************
 * Validates whether the given phone number is valid.
 * 
 * Does not enforce a specific length of the phone number.
 * Since the length varies by country.
 * 
 * inputs:
 * - phone - The phone number to check.
 * outputs:
 * - 1 if the phone number is valid otherwise 0.
 *******************************************************************************/
int is_valid_phone(const char *phone) {
    /* Check if the phone number is valid */
    if (phone == NULL) {
        return 0;
    }

    /* Return whether the phone number is all digits */
    return isAllDigits(phone);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <application/hospital.h>
#include "utils/scanner.h"
#include "utils/hash.h"

/*******************************************************************************
 * Validates whether the given character is a digit.
 *
 * inputs:
 * - character - The character to check.
 * outputs:
 * - 1 if the character is a digit otherwise 0.
 ******************************************************************************/
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
 * - 0 if the string is all digits otherwise 1.
 ******************************************************************************/
int isAllDigits(const char *string)
{
	/* Iterate through the given string */
	int i;
	for (i = 0; i < strlen(string); i++)
	{
		/* Ensure each character is a number between 0 and 9 */
		if (isDigit(string[i]) == 0)
		{
			/* 1 will be returned if the character is not a number */
			return 1;
		}
	}

	/* 0 will be returned since no issues were found */
	return 0;
}

/*******************************************************************************
 * Validates whether the given email is valid.
 *
 * inputs:
 * - email - The email to check.
 * outputs:
 * - 1 if the email is invalid otherwise 0.
 ******************************************************************************/
int is_valid_email(const char *email) {
    /* Check if the email is valid */
    if (email == NULL) {
        return 1;
    }

    /* Get the email domain */
    char *domain_part = strchr(email, '@');

    /* If domain part is found */
    if (domain_part != NULL) {

        /* If domain contains a dot */
        if (strchr(domain_part, '.') != NULL) {
            /* The email should be considered valid */
            return 0;
        }
    }

    /* The email should not be considered valid */
    return 1;
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
 * - 0 if the phone number is valid otherwise 1.
 ******************************************************************************/
int is_valid_phone(const char *phone) {
    /* Check if the phone number is valid */
    if (phone == NULL) {
        return 1;
    }

    /* Return whether the phone number is all digits */
    return isAllDigits(phone);
}

/*******************************************************************************
 * Validates whether the given blood type is valid.
 * 
 * inputs:
 * - blood_type - The blood type to check.
 * outputs:
 * - 1 if the blood type is valid otherwise 0.
 ******************************************************************************/
int is_valid_blood_type(const char blood_type[256]) {
    /* Check if the blood type is valid */
    if (blood_type == NULL) {
        return 0;
    }

    /* Convert the blood type to lowercase */
    char blood_type_lower[256];
    int i;
    for (i = 0; i < strlen(blood_type); i++) {

        /* Set the current character */
        blood_type_lower[i] = blood_type[i];

        /* If character is uppercase, convert to lowercase */
        if (blood_type[i] >= 'A' && blood_type[i] <= 'Z') {
            /* The lowercase equivalent is 32 places ahead in the ASCII table */
            blood_type_lower[i] = blood_type_lower[i] + 32;
        }
    }
    
    /* Array of valid blood types */
    char *valid_blood_types[] = {"a", "b", "ab", "o"};

    /* Check if the blood type is valid */
    for (i = 0; i < 4; i++) {
        if (strcmp(blood_type_lower, valid_blood_types[i]) == 0) {
            return 1;
        }
    }

    /* Return 0 if the blood type is not valid */
    return 0;
}


/*******************************************************************************
 * Validates the name given by the user.
 * 
 * inputs:
 * - name - The string to store the name .
 * outputs:
 * - 0 if the name is valid otherwise 1.
 ******************************************************************************/
int validate_name(char *name) {

    /* If the name is empty */
    if ( name == NULL || strlen(name) == 0) {
        printf("Name cannot be empty\n");
        return 1;
    }

    /* If no space is found in the name, the full name is not given */
    if (strchr(name, ' ') == NULL) {
        printf("Please enter your full name\n");
        return 1;
    }

    /* Return 0 if validation passes */
    return 0;
}


/*******************************************************************************
 * Asks for the user's full name.
 * 
 * inputs:
 * - name - The string to store the name .
 * - update - Whether this is an update.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_name(
    char *name,
    int update) {

    /* Iterate indefinately */
    while (1) {

        /* If this is an update, print the word New*/
        if (update == 1) {
            printf("New ");
        }

        /* Ask for the name */
        read_string("Name: ", name, sizeof(name));

        /* Exit once the name is valid */
        if (validate_name(name) == 0) {
            break;
        }
    }
}

/*******************************************************************************
 * Asks for the user's email.
 * 
 * inputs:
 * - email - The string to store the email .
 * - update - Whether this is an update.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_email(
    char *email,
    int update) {
    /* Iterate indefinately */
    while (1) {

        /* If updating, should be prefixed with "New " */
        printf("%s", update == 1 ? "New " : "");

        /* Ask for the email */
        read_string("Email: ", email, sizeof(email));

        /* Exit once the email is valid */
        if (is_valid_email(email) == 0) {
            break;
        }

        /* Print the error message */
        printf("Invalid email\n");
    }
}

/*******************************************************************************
 * Asks for the user's phone number.
 * 
 * inputs:
 * - phone - The string to store the phone number .
 * - update - Whether this is an update.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_phone(
    char *phone,
    int update) {

    /* Iterate indefinately */
    while (1) {

        /* If updating, should be prefixed with "New " */
        printf("%s", update == 1 ? "New " : "");

        /* Ask for the phone number */
        read_string("Phone: ", phone, sizeof(phone));

        /* Exit once the phone number is valid */
        if (is_valid_phone(phone) == 0) {
            break;
        }

        /* Print the error message */
        printf("Invalid phone number\n");
    }
}

/*******************************************************************************
 * Validates the password given by the user.
 * 
 * inputs:
 * - password - The string to store the password .
 * outputs:
 * - 0 if the password is valid otherwise 1.
 ******************************************************************************/
int validate_password(char *password) {

    /* If the name is empty */
    if ( password == NULL || strlen(password) == 0) {
        printf("Password cannot be empty\n");
        return 1;
    }

    /* Ensure the password is at least 8 characters long */
    if (strlen(password) < 8) {
        printf("Password must be at least 8 characters long\n");
        return 1;
    }

    /* Ensure the password contains at least one uppercase letter */
    if (!strpbrk(password, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
        printf("Password must contain at least one uppercase letter\n");
        return 1;
    }

    /* Ensure the password contains at least one lowercase letter */
    if (!strpbrk(password, "abcdefghijklmnopqrstuvwxyz")) {
        printf("Password must contain at least one lowercase letter\n");
        return 1;
    }

    /* Ensure the password contains at least one digit */
    if (!strpbrk(password, "0123456789")) {
        printf("Password must contain at least one digit\n");
        return 1;
    }

    /* Ensure the password contains at least one special character */
    if (!strpbrk(password, "!@#$")) {
        printf("Password must contain at least one special character\n");
        return 1;
    }

    /* Return 0 if validation passes */
    return 0;
}


/*******************************************************************************
 * Asks for the user's password.
 * 
 * inputs:
 * - update - Whether this is an update.
 * outputs:
 * - The hashed password.
 ******************************************************************************/
unsigned int ask_for_password( int update) {

    /* Description of acceptable passwords */
    char password_prompt[256];
    strcpy(password_prompt, "Passwords must be:\n");
    strcat(password_prompt, "- At least 8 characters long\n");
    strcat(password_prompt, "- Contain at least one uppercase letter\n");
    strcat(password_prompt, "- Contain at least one lowercase letter\n");
    strcat(password_prompt, "- Contain at least one digit\n");
    strcat(password_prompt, "- Contain at least one special character(!@#$)\n");
    strcat(password_prompt, "Enter your password: ");

    /* Iterate indefinately */
    char temp_password[256];
    while (1) {

        /* If updating, should be prefixed with "New " */
        printf("%s", update == 1 ? "New " : "");

        /* Ask for the password */
        read_string(password_prompt, temp_password, sizeof(temp_password));

        /* Exit once the password is valid */
        if (validate_password(temp_password) == 0) {
            break;
        }
    }

    /* Hash the password */
    return hash_string(temp_password);
}

/*******************************************************************************
 * Asks for the user's blood type.
 * 
 * inputs:
 * - blood_type - The string to store the blood type .
 * - update - Whether this is an update.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_blood_type(
    char *blood_type,
    int update) {

    /* Iterate indefinately */
    while (1) {

        /* If updating, should be prefixed with "New " */
        printf("%s", update == 1 ? "New " : "");

        /* Ask for the blood type */
        read_string("Blood type: ", blood_type, sizeof(blood_type));

        /* Exit once the blood type is valid */
        if (is_valid_blood_type(blood_type) == 0) {
            break;
        }
    }
}

/*******************************************************************************
 * Asks for the user's weight.
 * 
 * inputs:
 * - update - Whether this is an update.
 * outputs:
 * - The weight as a float.
 ******************************************************************************/
float ask_for_weight(int update) {

    /* Prompt for the weight */
    char prompt[256];

    /* If updating, should be prefixed with "New " */
    if (update == 1) {
        strcpy(prompt, "New Weight(kg): ");
    } else {
        strcpy(prompt, "Weight(kg): ");
    }

    /* Return the weight */
    return read_float(prompt);
} 

/*******************************************************************************
 * Asks for the user's height.
 * 
 * inputs:
 * - update - Whether this is an update.
 * outputs:
 * - The height as a float.
 ******************************************************************************/
float ask_for_height(int update) {

    /* Prompt for the height */
    char prompt[256];

    /* If updating, should be prefixed with "New " */
    if (update == 1) {
        strcpy(prompt, "New Height(cm): ");
    } else {
        strcpy(prompt, "Height(cm): ");
    }

    /* Return the height */
    return read_float(prompt);
} 


/*******************************************************************************
 * Asks for the user's medical history.
 * 
 * inputs:
 * - existing_medical_history - The existing medical history.
 * - new_medical_history - The new medical history.
 ******************************************************************************/
void ask_for_medical_history(const char *existing_medical_history, char *new_medical_history) {

    /* If the existing medical history is not empty, print it */
    if (existing_medical_history != NULL && strlen(existing_medical_history) > 0) {
        printf("Existing Medical History: %s\n", existing_medical_history);

        /* This is an update so "New" should be prefixed */
        printf("New ");
    }

    /* Prompt for the medical history */
    char prompt[256];
    strcpy(prompt, "Medical History: ");

    /* Ask for the medical history */
    char temp_medical_history[256];
    read_string(prompt, temp_medical_history, sizeof(temp_medical_history));

    /* Create the new medical history */
    strcpy(new_medical_history, existing_medical_history);
    strcat(new_medical_history, temp_medical_history);
}



#include <string.h>

/*******************************************************************************
 * Check if the given character is a letter.
 * A letter is any character between a-z or A-Z.
 * 
 * inputs:
 * - c - The character to check
 * 
 * outputs:
 * - 1 if the character is a letter, 0 otherwise
 *******************************************************************************/
int is_letter(char c) {

    /* If the character is between a-z, return 1 */
    if (c >= 'a' && c <= 'z') {
        return 1;
    }

    /* If the character is between A-Z, return 1 */
    if (c >= 'A' && c <= 'Z') {
        return 1;
    }

    /* Any other character is not a letter */
    return 0;
}

/*******************************************************************************
 * Check if the given character is a number.
 * A number is any character between 0-9.
 * 
 * inputs:
 * - c - The character to check
 * 
 * outputs:
 * - 1 if the character is a number, 0 otherwise
 *******************************************************************************/
int is_number(char c) {

    /* If the character is between 0-9, return 1 */
    return c >= '0' && c <= '9';
}

/*******************************************************************************
 * Check if the given string is only letters. 
 * A letter is any character between a-z or A-Z.
 * 
 * inputs:
 * - str - The string to check
 * 
 * outputs:
 * - 1 if the string is only letters, 0 otherwise
 *******************************************************************************/
int is_only_letters(const char *str) {

    /* Loop through the string */
    int i;
    for (i = 0; i < strlen(str); i++) {

        /* If the character is not a letter, return 0 */
        if (!is_letter(str[i])) {
            return 0;
        }
    }

    /* Return 1 if the string is only letters */
    return 1;
}

/*******************************************************************************
 * Check if the given string is only numbers.
 * A number is any character between 0-9.
 * 
 * inputs:
 * - str - The string to check
 * 
 * outputs:
 * - 1 if the string is only numbers, 0 otherwise
 ******************************************************************************/
int is_only_numbers(const char *str) {

    /* Loop through the string */
    int i;
    for (i = 0; i < strlen(str); i++) {
        /* If the character is not a number, return 0 */
        if (!is_number(str[i])) {
            return 0;
        }
    }

    /* Return 1 if the string is only numbers */
    return 1;
}

/*******************************************************************************
 * Check if the given string is a valid email.
 * 
 * inputs:
 * - email - The email to check
 * outputs:
 * - 1 if the email is valid, 0 otherwise
 ******************************************************************************/
int is_valid_email(const char *email) {

    /* Get the string after the '@' symbol */
    char *domain_part = strchr(email, '@');

    /* If the '@' symbol is not found, return 0 */
    if (domain_part == NULL) {
        return 0;
    }

    /* Get the string after the '.' symbol */
    domain_part = strchr(email, '.');

    /* If the '.' symbol is not found, return 0 */
    if (domain_part == NULL) {
        return 0;
    }

    /* Consider the email valid both '@' and '.' are present */
    return 1;
}

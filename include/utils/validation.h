#ifndef UTILS_VALIDATION_H
#define UTILS_VALIDATION_H



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
int is_letter(char c);

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
int is_number(char c);

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
int is_only_letters(const char *str);

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
int is_only_numbers(const char *str);

/*******************************************************************************
 * Check if the given string is a valid email.
 * 
 * inputs:
 * - email - The email to check
 * outputs:
 * - 1 if the email is valid, 0 otherwise
 ******************************************************************************/
int is_valid_email(const char *email);



#endif


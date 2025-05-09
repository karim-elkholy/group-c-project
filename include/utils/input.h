#ifndef UTILS_INPUT_H
#define UTILS_INPUT_H

/*******************************************************************************
 * Validates whether the given email is valid.
 *
 * inputs:
 * - email - The email to check.
 * outputs:
 * - 1 if the email is valid otherwise 0.
 *******************************************************************************/
int is_valid_email(const char *email);

/*******************************************************************************
 * Validates whether the given phone number is valid.
 *
 * inputs:
 * - phone - The phone number to check.
 * outputs:
 * - 1 if the phone number is valid otherwise 0.
 *******************************************************************************/
int is_valid_phone(const char *phone);

/*******************************************************************************
 * Validates whether the given password is valid.
 *
 * inputs:
 * - password - The password to check.
 * outputs:
 * - 1 if the password is valid otherwise 0.
 *******************************************************************************/
int is_valid_password(const char *password);

#endif


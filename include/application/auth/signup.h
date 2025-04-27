#ifndef APPLICATION_AUTH_SIGNUP_H
#define APPLICATION_AUTH_SIGNUP_H

/*******************************************************************************
 * Asks for a name-like input and ensures it is valid.
 * 
 * inputs:
 * - prompt - The prompt to display to the user.
 * - name - Stores the given name.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_name(const char *prompt, char *name);

/*******************************************************************************
 * Asks for a password and confirms it.
 * 
 * inputs:
 * - password - Stores the given password.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_password(char *password);

/*******************************************************************************
 * Asks for an email.
 * 
 * inputs:
 * - email - Stores the given email.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_email(char *email);


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
void ask_for_phone(char *phone);
#endif



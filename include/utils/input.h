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


/*******************************************************************************
 * Asks for the user's full name.
 * 
 * inputs:
 * - name - The string to store the name .
 * - update - Whether this is an update.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_name(char *name, int update);

/*******************************************************************************
 * Asks for the user's email.
 * 
 * inputs:
 * - email - The string to store the email.
 * - update - Whether this is an update.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_email(char *email, int update);

/*******************************************************************************
 * Asks for the user's phone number.
 * 
 * inputs:
 * - phone - The string to store the phone number.
 * - update - Whether this is an update.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_phone(char *phone, int update);

/*******************************************************************************
 * Asks for the user's password.
 * 
 * inputs:
 * - update - Whether this is an update.
 * outputs:
 * - The hashed password.
 ******************************************************************************/
unsigned int ask_for_password( int update);

/*******************************************************************************
 * Asks for the user's blood type.
 * 
 * inputs:
 * - blood_type - The string to store the blood type.
 * - update - Whether this is an update.
 * outputs:
 * - none
 ******************************************************************************/
void ask_for_blood_type(char *blood_type, int update);

/*******************************************************************************
 * Asks for the user's weight.
 * 
 * inputs:
 * - update - Whether this is an update.
 * outputs:
 * - The weight as a float.
 ******************************************************************************/
float ask_for_weight(int update);

/*******************************************************************************
 * Asks for the user's height.
 * 
 * inputs:
 * - update - Whether this is an update.
 * outputs:
 * - The height as a float.
 ******************************************************************************/
float ask_for_height(int update);

/*******************************************************************************
 * Asks for the user's medical history.
 * 
 * inputs:
 * - existing_medical_history - The existing medical history.
 * - new_medical_history - The new medical history.
 ******************************************************************************/
void ask_for_medical_history(const char *existing_medical_history, char *new_medical_history);











#endif


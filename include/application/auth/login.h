#ifndef APPLICATION_AUTH_LOGIN_H
#define APPLICATION_AUTH_LOGIN_H

/*******************************************************************************
 * Verifies the user's password.
 * 
 * inputs:
 * - expected_pass_hash - The expected password hash.
 * outputs:
 * - 1 if the password is correct, otherwise 0
 ******************************************************************************/
int verify_user_password(unsigned int expected_pass_hash);

#endif


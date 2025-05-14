#ifndef UTILS_SCANNER_H
#define UTILS_SCANNER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*******************************************************************************
 * Used to read a string from the user.
 * 
 * inputs:
 * - prompt - The prompt to display to the user
 * - input - String to store the user's input
 * - input_size - Size of the string
 * outputs:
 * - The user's input
 ******************************************************************************/
char *read_string(const char *prompt, char *input, int input_size);

/*******************************************************************************
 * Used to read input from the user. Enforces a single character input.
 *
 * inputs:
 * - none
 * outputs:
 * - The user's choice
 ******************************************************************************/
char read_choice(const char *prompt);

/*******************************************************************************
 * Used to read a float from the user.
 * 
 * inputs:
 * - prompt - The prompt to display to the user
 * - input - String to store the user's input
 * - input_size - Size of the string
 * outputs:
 * - The user's input
 ******************************************************************************/
float read_float(const char *prompt);
#endif


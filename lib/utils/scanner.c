
#include "utils/scanner.h"

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
char *read_string(const char *prompt, char *input, int input_size) {

	/* Write the string to a large buffer first */
	char buffer[1024];

	/* Print the prompt to the user */
    printf("%s", prompt);

	/* Exit with an error code if the user's input could not be read */
	if (fgets(buffer, sizeof(buffer), stdin) == NULL)
	{
		exit(1);
	}

	/* Remove the newline character captured as part of fgets */
	buffer[strcspn(buffer, "\n")] = '\0';

	/* If the buffer is too large, exit with an error code */
	if (strlen(buffer) >= input_size)
	{
		/* Return an empty string */
		input[0] = '\0';
		return input;
	}

	/* Copy the buffer to the input */
	strcpy(input, buffer);

	/* Return the input */
	return input;
}

/*******************************************************************************
 * Used to read input from the user. Enforces a single character input.
 *
 * inputs:
 * - none
 * outputs:
 * - The user's choice
 ******************************************************************************/
char read_choice(const char *prompt)
{
	/* Read the user's choice
	 * The user might enter a sentence instead of a single character
	 * To minimise the chance of buffer overflow, 256 characters are used */
	char choice[256];

    /* Capture the user's input */
    read_string(prompt, choice, sizeof(choice));

	/* If more than a single character is entered */
	if (strlen(choice) > 1)
	{
		/* Return an invalid character
		 * This will force the user to enter a valid choice
		 * A valid choice is a single character */
		return '\0';
	}

	/* Return the first character */
	return choice[0];
}

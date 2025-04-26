
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
void readString(const char *prompt, char *input, int input_size) {

    /* Print the prompt to the user */
    printf("%s", prompt);

	/* Exit with an error code if the user's input could not be read */
	if (fgets(input, input_size, stdin) == NULL)
	{
		exit(1);
	}

	/* Remove the newline character captured as part of fgets */
	input[strcspn(input, "\n")] = '\0';
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
    readString(prompt, choice, sizeof(choice));

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

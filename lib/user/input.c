
#include "user/core.h"
#include "user/input.h"
/* Functions used: malloc, free */
#include <stdlib.h>

/* Functions used: printf, fopen, fclose, fprintf, fscanf */
#include <stdio.h>

/* Functions used: strlen, strcpy, strncpy, sprintf */
#include <string.h>

/*******************************************************************************
 * Main
 *******************************************************************************/
int main(void)
{

	/* Settings
	 * The name of the database file */
	char dbFileName[] = "database";

	/* Use the program */
	use(dbFileName);

	return 0;
}

/*******************************************************************************
 * Function to use the library command-line program.
 *
 * inputs:
 * - dbFileName - The name of the database file
 * outputs:
 * - none
 *******************************************************************************/
void use(const char *dbFileName)
{
	/* Show the available choices */
	printMenu();

    //todo replace with patients and doctors instead of books

	/* Allocate memory for the books */
	book_t *books = (book_t *)malloc(MAX_LIBRARY_SIZE * sizeof(book_t));
	int book_count = 0;

	/* Get the user's choice */
	char choice;
	while ((choice = readChoice()) != '6')
	{

		/* Process the menu choice */
		switch (choice)
		{
		case '1':
			addBook(books, &book_count);
			break;
		case '2':
			/* Delete the last book */
			deleteLastBook(&book_count);
			break;
		case '3':
			/* Display the book list */
			displayBookList(books, book_count);
			break;
		case '4':
			/* Save the book list to the database */
			saveBookList(books, book_count, dbFileName);
			break;
		case '5':
			/* Read the book list from the database */
			readBookList(books, &book_count, dbFileName);
			break;
		default:
			printf("Invalid choice.\n");
			break;
		}
		/* Show the menu again after each operation */
		printMenu();
	}

	/* Once the user has exited the program, free the allocated memory
	 * Failing to do this will lead to memory leaks */
	free(books);
}

/*******************************************************************************
 * Used to read the user's choice.
 *
 * inputs:
 * - none
 * outputs:
 * - The user's choice
 *******************************************************************************/
char readChoice(void)
{

	/* Read the user's choice
	 * The user might enter a sentence instead of a single character
	 * To minimise the chance of buffer overflow, 256 characters are used */
	char choice[256];
	printf("Enter your choice>\n");

	/* Exit with an error code if the user's input could not be read */
	if (fgets(choice, sizeof(choice), stdin) == NULL)
	{
		exit(1);
	}

	/* Remove the newline character captured as part of fgets */
	choice[strcspn(choice, "\n")] = '\0';

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

/*******************************************************************************
 * This function prints the initial menu with all instructions on how to use
 * this program.
 * inputs:
 * - none
 * outputs:
 * - none
 *******************************************************************************/
void printMenu(void)
{
	printf("\nLibrary Management System \n"
		   "1. Add book\n"
		   "2. Delete last book\n"
		   "3. Display book list\n"
		   "4. Save the book list to the database\n"
		   "5. Read the book list from the database\n"
		   "6. Exit the program\n");
}

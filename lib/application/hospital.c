#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/scanner.h"
#include "application/database.h"
#include "application/users/doctor.h"
#include "application/users/patients.h"


/*******************************************************************************
 * Print the available choices.
 *
 * inputs:
 * - none
 * outputs:
 * - none
 ******************************************************************************/
void print_menu() {
    printf("P. Login as Patient\n");
    printf("D. Login as Doctor\n");
    printf("M. Print this menu again\n");
    printf("X. Exit\n");
}

 /******************************************************************************
 * Entry point for the app.
 *
 * inputs:
 * - hospital_name - The name of the hospital.
 * outputs:
 * - none
 ******************************************************************************/
void use(const char *hospital_name)
{
    /* Print a welcome message */
    printf("Welcome to the %s hospital management system\n", hospital_name);

    /* Load the database */
    int num_users;
    record_t *records = load_database(hospital_name, &num_users);

    // Show the available choices
    print_menu();

	/* Get the user's choice */
	char choice;
	while ((choice = read_choice("Enter your choice>\n")) != 'X')
	{
		/* Process the menu choice */
		switch (choice)
		{
            case 'P':
                /* Use the patient menu */
                break;
            case 'D':
                /* Use the doctor menu */
                doctor_login(records);
                break;
            case 'M':
                print_menu();
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
	}

    /* Print a goodbye message */
    printf("Thank you for using the %s hospital management system\n", hospital_name);

	/* Once the user has exited the program, free the allocated memory
	 * Failing to do this will lead to memory leaks */
	free(records);
}


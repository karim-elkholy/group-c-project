#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "application/database.h"
#include "application/users/doctor.h"
#include "utils/hash.h"
#include "utils/scanner.h"


/*******************************************************************************
 * Verifies the user's password.
 * 
 * inputs:
 * - user - The user to verify
 * outputs:
 * - 1 if the password is correct, otherwise 0
 ******************************************************************************/
int verify_user_password(unsigned int expected_password) {

    /* Counter for keeping track of the number of login attempts */
    int counter = 3;

    while ( counter != 0 ) {

        /* Store the user's password */
        char user_password[256];
        read_string("Password: ", 
            user_password, sizeof(user_password));

        /* Consider the user verified if the password is correct */
        if (expected_password == hash_string(user_password)) {
            return 1;
        }

        /* Increment the counter each time an incorrect password is entered */
        counter -= 1;
        printf("Incorrect password, %d attempts remaining\n", counter);
    }

    /* False if the password could not be verified.*/
    return 0;
}

/*******************************************************************************
 * Handles access to the 'patient' menu.
 * Logs in a patient or provides signup option.
 * 
 * inputs:
 * - users - The database
 * - num_users - The number of users in the database
 ******************************************************************************/
void use_patient_menu(hospital_record_t *records) {

    /* TODO - Implement the patient menu */
}


/*******************************************************************************
 * Handles access to the 'patient' menu.
 * Ensures user is authenticated before providing access. 
 * 
 * inputs:
 * - users - The database
 * - num_users - The number of users in the database
 * outputs:
 * - none
 ******************************************************************************/
void use_doctor_menu(hospital_record_t *records) {

    /* If no doctors exist */
    if (records->num_doctors == 0) {

        /* Explain why the user needs to sign up as a doctor */
        printf("First-time Setup: Doctor Registration Required\n");
        printf("Please signup as a doctor to initialize the system.\n");

        /* Sign up as a doctor */
        doctor_signup(records);

        /* Redirecting to login menu message*/
        printf("Redirecting to login menu\n");
    }

    /* Stylish login message*/
    printf("--------------------------------\n");
    printf("Doctor Login \n");
    printf("--------------------------------\n");

    /* Store the user's ID */
    char user_id[256];
    read_string("Username: ", user_id, sizeof(user_id));
    
    /* Find the user */
    doctor_details_t *doctor = find_doctor(
        records, user_id);

    /* If the user is not found, print an error message */
    if (doctor == NULL) {
        printf("No doctor with ID %s found\n", user_id);
        return;

    }

    /* Login to the doctor menu if the user provides the correct password */
    if (verify_user_password(doctor->password) == 1) {
        /* Call the doctor menu */
        /* doctor_menu(doctor); */
    }
}


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

/*******************************************************************************
 * Seed data function.
 * 
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 ******************************************************************************/
void seed_data(hospital_record_t *records) {
    /* Add a doctor to the hospital records */
    doctor_details_t *doctor = (doctor_details_t *)calloc(
        1, sizeof(doctor_details_t)
    );
    strcpy(doctor->username, "1");
    strcpy(doctor->name, "John Doe");
    strcpy(doctor->email, "john.doe@example.com");
    strcpy(doctor->phone, "1234567890");
    doctor->password = hash_string("1");
    strcpy(doctor->specialization, "Cardiology");
    strcpy(doctor->license_number, "1234567890");
    doctor_signup_silent(records, doctor);
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
    hospital_record_t *records = load_database(hospital_name);

    /* Show the available choices */
    print_menu();

	/* Get the user's choice */
	char choice;
	while ((choice = read_choice("Enter your choice> ")) != 'X')
	{
		/* Process the menu choice */
		switch (choice)
		{
            case 'P':
                use_patient_menu(records);
                break;
            case 'D':
                use_doctor_menu(records);
                break;
            case 'M':
                print_menu();
                break;
            default:
                printf("Invalid choice\n");
                break;
        }

        /* Save the database */
        save_database(records);

	}

    /* Print a goodbye message */
    printf("Thank you for using the %s hospital management system\n", hospital_name);

	/* Once the user has exited the program, free the allocated memory
	 * Failing to do this will lead to memory leaks */
	close_database(records);
}


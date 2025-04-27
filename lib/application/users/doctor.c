#include <stdio.h>
#include <string.h>

#include "utils/scanner.h"
#include "application/users/doctor.h"
#include "application/users/records.h"
#include "application/auth/signup.h"
#include "application/auth/login.h"

/*******************************************************************************
 * Prints the doctor menu.
 * 
 * inputs:
 * - none
 * outputs:
 * - none
 *******************************************************************************/
void print_doctor_menu()
{
	printf("\n Doctor Menu \n"
		   "1. Register a new patient\n"
		   "2. View all patients\n"
		   "3. View a patient's details\n"
		   "4. Delete a patient\n"
		   "5. Add a new doctor\n"
		   "6. Exit\n");
}


/*******************************************************************************
 * Finds a doctor in the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * - num_users - The number of users in the database
 * - user_id - The ID of the user to find
 * outputs:
 * - The user if found, otherwise NULL
 ******************************************************************************/
doctor_details_t *find_doctor(record_t *records, char *id) {

    /* Iterate through the doctors */
    int i;
    for (i = 0; i < records->num_doctors; i++) {

        /* If this user's ID matches the given ID */
        if (strcmp(records->doctors[i].user.id, id) == 0) {
            return &records->doctors[i];
        }
    }

    /* If no doctor is found, return NULL */
    return NULL;
}


/*******************************************************************************
 * Handles access to the 'doctor' menu.
 * Ensures user is authenticated before providing access. 
 * 
 * inputs:
 * - records - The database of users.
 * outputs:
 * - The doctor if found, otherwise NULL
 ******************************************************************************/
void doctor_login(record_t *records) {

    /* Store the user's ID */
    char user_id[256];
    readString("Enter your ID: ", user_id, sizeof(user_id));

    /* Find the user */
    doctor_details_t *doctor = find_doctor(records, user_id);

    /* If the user is not found, print an error message */
    if (doctor == NULL) {
        printf("No doctor with ID %s found\n", user_id);
        return;
    }

    /* Exit early if the user failed to provide the correct password */
    if (verify_user_password(doctor->user.password) == 0) {
        return;
    }

    /* Call the doctor menu */
    doctor_use(doctor);
}


/*******************************************************************************
 * Entry point to use the 'doctor' menu.
 * 
 * inputs:
 * - none
 * outputs:
 * - none
 ******************************************************************************/
void doctor_use(doctor_details_t *doctor) {

    /* Start the login process for the doctor */
    /*doctor_details_t *doctor = doctor_login(records);*/

    /* If the doctor is not found, print an error message */
    if (doctor == NULL) {
        return;
    }

    /* Welcome the doctor */
    printf("Welcome, Dr.%s\n", doctor->user.name);

    /* Print the doctor menu */
    print_doctor_menu();

    /* Get the user's choice */
    char choice;
    while ((choice = read_choice("Enter your choice>\n")) != 'X')
    {
        /* Process the menu choice */
        switch (choice)
        {
            case '1':
                /*create_patient_interactive();*/
                break;
            case '2':
                /*view_all_patients();*/
                break;
        }
    }

    /* Say goodbye to the doctor */
    printf("Goodbye, Dr.%s\n", doctor->user.name);
}

void create_doctor_interactive() {

    /* Get the name and ID of the doctor*/
    char name[256];
    char id[256];
    readString("Enter your name: ", name, sizeof(name));
    readString("Enter your ID: ", id, sizeof(id));

    /* Get the password of the doctor */
    char password[256];
    ask_for_password(password);

    /* Get the email of the doctor */
    char email[256];
    ask_for_email(email);

    /* Get the phone number of the doctor */
    char phone[256];
    ask_for_phone(phone);

    /* Ask for the specialization of the doctor */
    char specialization[256];
    readString("Enter your specialization: ", 
        specialization, sizeof(specialization));
    
    /* Ask for the license number of the doctor */
    char license_number[256];
    readString("Enter your license number: ", 
        license_number, sizeof(license_number)
    );
}

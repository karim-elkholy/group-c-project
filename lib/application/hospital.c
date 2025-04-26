 #include <stdio.h>
 #include <stdlib.h>
 #include "input.h"
 #include "user/core.h"
#include "application/users.h"

user_record_t *load_database(const char *hospital_name, int *num_users);
void save_database(const char *hospital_name, user_record_t *records);

/*******************************************************************************
 * Verifies the user's password.
 * 
 * inputs:
 * - user - The user to verify
 * outputs:
 * - 1 if the password is correct, otherwise 0
 ******************************************************************************/
int verify_user_password(const char *expected_password) {

    /* Counter for keeping track of the number of login attempts */
    int counter = 3;

    while ( counter != 0 ) {

        /* Store the user's password */
        char user_password[256];
        read_string("Enter your password: ", user_password, sizeof(user_password));

        /* Consider the user verified if the password is correct */
        if (strcmp(expected_password, hash_text(user_password)) == 0) {
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
 * Handles access to the 'doctor' menu.
 * Ensures user is authenticated before providing access. 
 * 
 * inputs:
 * - users - The database
 * - num_users - The number of users in the database
 * outputs:
 * - none
 ******************************************************************************/
void use_doctor_menu(user_record_t *records) {

    /* Store the user's ID */
    char user_id[256];
    read_string("Enter your ID: ", user_id, sizeof(user_id));

    /* Find the user */
    doctor_details_t *doctor = find_doctor(records, user_id);

    /* If the user is not found, print an error message */
    if (doctor == NULL) {
        printf("No doctor with ID %s found\n", user_id);
        return;
    }

    /* Exit early if the user failed to provide the correct password */
    if (verify_user_password(doctor->password) == 0) {
        return;
    }

    /* Call the doctor menu */
    doctor_menu(doctor);
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
    user_record_t *records = load_database(hospital_name, &num_users);

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
                patient_use();
                break;
            case 'D':
                doctor_use();
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
 * Load the database.
 * 
 * inputs:
 * - hospital_name - The name of the hospital.
 * outputs:
 * - The database.
 ******************************************************************************/
user_record_t *load_database(const char *hospital_name, int *num_users) {

    /* Allocate list of users */
    user_record_t *records = (user_record_t *)malloc(sizeof(user_record_t));
    
    /* Name of database file */
    char db_name[256];
    sprintf(db_name, "%s.db", hospital_name);

    /* Open the database file */
    FILE *db = fopen(db_name, "r");

    /* If the database file was not opened successfully */
    if (db == NULL) {
        /* Assume the database is empty */
        *num_users = 0;
        return records;
    }

    /* TODO - Later decrypt the database */

    /* Set the number of users in the database */
    fscanf(db, "%d", &records->num_patients);

    /* Read all the patients from the database */
    int i;
    for (i = 0; i < records->num_patients; i++) {
        fscanf(db, "%s", records->patients[i].id);
        fscanf(db, "%s", records->patients[i].name);
        fscanf(db, "%s", records->patients[i].email);
        fscanf(db, "%s", records->patients[i].phone);
        fscanf(db, "%s", records->patients[i].password);

        fscanf(db, "%s", records->patients[i].blood_type);
        fscanf(db, "%s", records->patients[i].medical_history);
        fscanf(db, "%s", records->patients[i].allergies);
        fscanf(db, "%s", records->patients[i].medications);
        fscanf(db, "%f", &records->patients[i].weight);
        fscanf(db, "%f", &records->patients[i].height);
        fscanf(db, "%s", records->patients[i].notes);
    }

    /* Read all the doctors from the database */
    fscanf(db, "%d", &records->num_doctors);
    for (i = 0; i < records->num_doctors; i++) {
        fscanf(db, "%s", records->doctors[i].id);
        fscanf(db, "%s", records->doctors[i].name);
        fscanf(db, "%s", records->doctors[i].email);
        fscanf(db, "%s", records->doctors[i].phone);
        fscanf(db, "%s", records->doctors[i].password);
        fscanf(db, "%s", records->doctors[i].specialization);
        fscanf(db, "%s", records->doctors[i].license_number);
    }

    /* Return the list of users */
    return records;
}

/*******************************************************************************
 * Save the database.
 * 
 * inputs:
 * - hospital_name - The name of the hospital.
 * - records - The hospital records.
 * outputs:
 * - None.
 *******************************************************************************/
void save_database(const char *hospital_name, user_record_t *records) {

    /* Name of database file */
    char db_name[256];
    sprintf(db_name, "%s.db", hospital_name);

    /* Open the database file */
    FILE *db = fopen(db_name, "w");

    /* Failure to open the database file should cause the program to exit.*/
    if (db == NULL) {
        printf("Error: Failed to open database file.\n");
        exit(1);
    }

    /* TODO - Later encrypt the database */

    /* Write the number of patients to the database */
    fprintf(db, "%d\n", records->num_patients);

    /* Write the patients to the database */
    int i;
    for (i = 0; i < records->num_patients; i++) {
        fprintf(db, "%s\n", records->patients[i].id);
        fprintf(db, "%s\n", records->patients[i].name);
        fprintf(db, "%s\n", records->patients[i].email);
        fprintf(db, "%s\n", records->patients[i].phone);
        fprintf(db, "%s\n", records->patients[i].password);
        fprintf(db, "%s\n", records->patients[i].blood_type);
        fprintf(db, "%s\n", records->patients[i].medical_history);
        fprintf(db, "%s\n", records->patients[i].allergies);
        fprintf(db, "%s\n", records->patients[i].medications);
        fprintf(db, "%f\n", records->patients[i].weight);
        fprintf(db, "%f\n", records->patients[i].height);
        fprintf(db, "%s\n", records->patients[i].notes);
    }

    /* Write the number of doctors to the database */
    fprintf(db, "%d\n", records->num_doctors);

    /* Write the doctors to the database */
    for (i = 0; i < records->num_doctors; i++) {
        fprintf(db, "%s\n", records->doctors[i].id);
        fprintf(db, "%s\n", records->doctors[i].name);
        fprintf(db, "%s\n", records->doctors[i].email);
        fprintf(db, "%s\n", records->doctors[i].phone);
        fprintf(db, "%s\n", records->doctors[i].password);
        fprintf(db, "%s\n", records->doctors[i].specialization);
        fprintf(db, "%s\n", records->doctors[i].license_number);
    }
}



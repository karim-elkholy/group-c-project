#include <stdio.h>
#include <string.h>

#include "application/database.h"
#include "application/users/doctor.h"
#include "application/users/patient.h"
#include "utils/scanner.h"
#include "utils/hash.h"


/*******************************************************************************
 * Silently adds a new doctor to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * - doctor - The doctor to add
 * outputs:
 * - none
 ******************************************************************************/
void doctor_signup_silent(hospital_record_t *records, doctor_details_t *doctor) {

    /* If this is the first doctor */
    if ( records->num_doctors == 0 ) {

        /* Add the doctor to the list */
        records->doctors = doctor;

        /* Mark the end of the list */
        records->doctors->next = NULL;
    
    /* If this is not the first entry */
    } else {

        /* Find the last doctor*/
        doctor_details_t *last_doctor = records->doctors;
        while (last_doctor->next != NULL) {
            last_doctor = last_doctor->next;
        }

        /* Add the new doctor to the end of the list */
        last_doctor->next = doctor;
    }

    /* Update the number of doctors */
    records->num_doctors += 1;
}

/*******************************************************************************
 * Interactively adds a new doctor to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 ******************************************************************************/
void doctor_signup(hospital_record_t *records) {

    printf("--------------------------------\n");
    printf("Doctor Signup \n");
    printf("--------------------------------\n");

    /* Ask the user for their details */
    char username[256];
    read_string("Enter your username: ", username, sizeof(username));
    /* Name */
    char name[256];
    read_string("Enter your name: ", name, sizeof(name));
    /* Email */
    char email[256];
    read_string("Enter your email: ", email, sizeof(email));
    /* Phone */
    char phone[256];
    read_string("Enter your phone: ", phone, sizeof(phone));
    /* Password */
    char password[256];
    read_string("Enter your password: ", password, sizeof(password));
    /* Specialization */
    char specialization[256];
    read_string("Enter your specialization: ", 
        specialization, 
        sizeof(specialization));
    /* License number */
    char license_number[256];
    read_string("Enter your license number: ", license_number, 
        sizeof(license_number));

    /* Create a new doctor */
    doctor_details_t *doctor = (doctor_details_t *)calloc(
        1, sizeof(doctor_details_t)
    );
    strcpy(doctor->username, username);
    strcpy(doctor->name, name);
    strcpy(doctor->email, email);
    strcpy(doctor->phone, phone);
    doctor->password = hash_string(password);
    strcpy(doctor->specialization, specialization);
    strcpy(doctor->license_number, license_number);

    /* Add the doctor to the hospital records */
    doctor_signup_silent(records, doctor);

    /* Print a success message if signup is successful */
    printf("Signup successful\n");
}


/*******************************************************************************
 * Adds a new patient to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 *******************************************************************************/
void add_patient(hospital_record_t *records) {

    /* Sign up a new patient */
    patient_signup(records);

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
doctor_details_t *find_doctor(
    hospital_record_t *records,
    char *user_id
) {

    /* Get the doctors from the hospital records */
    doctor_details_t *doctors = records->doctors;

    /* Iterate through the doctors */
    while ( doctors != NULL) {

        /* If this user's ID matches the given ID */
        if (strcmp(doctors->username, user_id) == 0) {
            return doctors;
        }

        /* Move to the next doctor */
        doctors = doctors->next;
    }

    /* If no doctor is found, return NULL */
    return NULL;
}

/*******************************************************************************
 * Prints the choices available for the doctor menu.
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
 * View all patients in the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 *******************************************************************************/
void view_all_patients(hospital_record_t *records) {

}

/*******************************************************************************
 * View a specific patient's details in the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 *******************************************************************************/
void view_patient_details(hospital_record_t *records) {

}

/*******************************************************************************
 * Entry point to use the 'doctor' menu.
 * 
 * inputs:
 * - none
 * outputs:
 * - none
 *******************************************************************************/
void doctor_use(hospital_record_t *records, doctor_details_t *doctor) {
    /* Print a welcome message to the doctor */
    printf("Welcome %s\n", doctor->name);

    // Show the available choices
    print_doctor_menu();

	/* Get the user's choice */
	char choice;
	while ((choice = read_choice("Enter your choice> ")) != 'X')
	{
		/* Process the menu choice */
		switch (choice)
		{
            case '1':
                patient_signup(records);
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                break;
            case '5':
                break;
            case '6':
                break;
            default:
                printf("Invalid choice\n");
                break;

            /* Update the records after every action */
            save_database(records);
        }
	}
}

void doctor_menu(doctor_details_t *doctor) {

}

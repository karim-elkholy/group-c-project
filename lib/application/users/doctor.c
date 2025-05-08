#include <stdio.h>
#include <string.h>

#include "application/database.h"
#include "application/users/doctor.h"
#include "utils/scanner.h"
#include "utils/hash.h"

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
 * Adds a new doctor to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 *******************************************************************************/
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
    read_string("Enter your specialization: ", specialization, sizeof(specialization));
    /* License number */
    char license_number[256];
    read_string("Enter your license number: ", license_number, sizeof(license_number));

    /* Create a new doctor */
    doctor_details_t doctor;
    strcpy(doctor.id, username);
    strcpy(doctor.name, name);
    strcpy(doctor.email, email);
    strcpy(doctor.phone, phone);
    doctor.password = hash_string(password);
    strcpy(doctor.specialization, specialization);
    strcpy(doctor.license_number, license_number);

    /* Add the doctor to the hospital records */
    records->doctors[records->num_doctors] = doctor;
    records->num_doctors += 1;

    /* Print a success message if signup is successful */
    printf("Signup successful\n");
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
doctor_details_t *find_doctor(doctor_details_t *records, int doctor_count, char *user_id) {

    /* Iterate through the doctors */
    int i;
    for (i = 0; i < doctor_count; i++) {

        /* If this user's ID matches the given ID */
        if (strcmp(records[i].id, user_id) == 0) {
            return &records[i];
        }
    }

    /* If no doctor is found, return NULL */
    return NULL;
}


/*******************************************************************************
 * Entry point to use the 'doctor' menu.
 * 
 * inputs:
 * - none
 * outputs:
 * - none
 *******************************************************************************/
void doctor_use() {

}


void create_doctor() {

}

void doctor_menu(doctor_details_t *doctor) {

}

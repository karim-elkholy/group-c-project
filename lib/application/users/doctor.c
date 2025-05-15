#include <stdio.h>
#include <string.h>

#include "application/database.h"
#include "application/users/doctor.h"
#include "application/users/patient.h"
#include "utils/scanner.h"
#include "utils/hash.h"
#include "utils/input.h"

/*******************************************************************************
 * Silently adds a new doctor to the hospital records.
 *
 * inputs:
 * - records - The hospital records
 * - doctor - The doctor to add
 * outputs:
 * - none
 ******************************************************************************/
void doctor_signup_silent(hospital_record_t *records, doctor_details_t *doctor)
{

    /* If this is the first doctor */
    if (records->num_doctors == 0)
    {
        /* Add the doctor to the list */
        records->doctors = doctor;

        /* Mark the end of the list */
        records->doctors->next = NULL;
    }
    /* If this is not the first entry */
    else
    {

        /* Find the last doctor */
        doctor_details_t *last_doctor = records->doctors;
        while (last_doctor->next != NULL)
        {
            last_doctor = last_doctor->next;
        }

        /* Add the new doctor to the end of the list */
        last_doctor->next = doctor;
    }

    /* Update the number of doctors */
    records->num_doctors += 1;
}
/*******************************************************************************
 * Validates the username.
 * 
 * inputs:
 * - records - The hospital records
 * - username - The string to store the username .
 * outputs:
 * - none
 ******************************************************************************/
int doctor_validate_username(hospital_record_t *records, char *username) {

    /* If the username is empty */
    if ( username == NULL || strlen(username) == 0) {
        printf("Username cannot be empty\n");
        return 1;
    }

    /* If the username contains a space */
    if (strchr(username, ' ') != NULL) {
        printf("Username cannot contain a space\n");
        return 1;
    }
    
    /* If another doctor has this username */
    if (find_doctor(records, username) != NULL) {
        printf("Doctor %s already exists\n", username);
        printf("Please choose a different username\n");
        return 1;
    }

    /* Return 0 if validation passes */
    return 0;
}

/*******************************************************************************
 * Asks for username.
 * 
 * inputs:
 * - records - The hospital records
 * - username - The string to store the username .
 * - update - Whether this is an update.
 * outputs:
 * - none
 ******************************************************************************/
void doctor_ask_for_username(
    hospital_record_t *records,
    char *username, 
    int update) {

    /* Iterate indefinately */
    while (1) {

        /* If this is an update, print the word New*/
        if (update == 1) {
            printf("New ");
        }

        /* Ask for the username */
        read_string("Username: ", username, sizeof(username));

        /* Exit once the username is valid */
        if (doctor_validate_username(records, username) == 0) {
            break;
        }
    }
}

/*******************************************************************************
 * Interactively adds a new doctor to the hospital records.
 *
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 ******************************************************************************/
void doctor_signup(hospital_record_t *records)
{

    printf("--------------------------------\n");
    printf("Doctor Signup \n");
    printf("--------------------------------\n");

    /* Ask the user for their details */
    char username[256];
    doctor_ask_for_username(records, username, 0);
    /* Name */
    char name[256];
    ask_for_name(name, 0);
    /* Email */
    char email[256];
    ask_for_email(email, 0);
    /* Phone */
    char phone[256];
    ask_for_phone(phone, 0);
    /* Password */
    unsigned int password = ask_for_password(0);
    /* Specialization */
    char specialization[256];
    read_string("Specialization: ",
                specialization,
                sizeof(specialization));
    /* License number */
    char license_number[256];
    read_string("License number: ", license_number,
                sizeof(license_number));

    /* Create a new doctor */
    doctor_details_t *doctor = (doctor_details_t *)calloc(
        1, sizeof(doctor_details_t));
    strcpy(doctor->username, username);
    strcpy(doctor->name, name);
    strcpy(doctor->email, email);
    strcpy(doctor->phone, phone);
    doctor->password = password;
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
void add_patient(hospital_record_t *records)
{

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
    char *user_id)
{

    /* Get the doctors from the hospital records */
    doctor_details_t *doctors = records->doctors;

    /* Iterate through the doctors */
    while (doctors != NULL)
    {

        /* If this user's ID matches the given ID */
        if (strcmp(doctors->username, user_id) == 0)
        {
            return doctors;
        }

        /* Move to the next doctor */
        doctors = doctors->next;
    }

    /* If no doctor is found, return NULL */
    return NULL;
}

/*******************************************************************************
 * Prints the choices available for the doctor update menu.
 *
 * inputs:
 * - none
 * outputs:
 * - none
 ******************************************************************************/
void print_doctor_update_menu()
{
    printf("\n Doctor Update Menu \n"
           "1. Username\n"
           "2. Name\n"
           "3. Email\n"
           "4. Phone\n"
           "5. Password\n"
           "6. Specialization\n"
           "7. License number\n"
           "X. Exit\n");
}

/*******************************************************************************
 * Updates a doctor's details in the hospital records.
 *
 * inputs:
 * - records - The hospital records
 * - doctor - The doctor to update
 *******************************************************************************/
void update_doctor_details(
    hospital_record_t *records, doctor_details_t *doctor)
{

    /* Get the user's choice */
    char choice;
    while ((choice = read_choice("Enter your choice> ")) != 'X')
    {
        /* Exit if requested*/
        if (choice == 'X')
        {
            break;
        }

        /* Process the menu choice */
        if (choice == '1')
        {

            /* Ask the user for the new username */
            char username[256];
            doctor_ask_for_username(records, username, 1);

            /* Update the doctor's username */
            strcpy(doctor->username, username);
        }
        else if (choice == '2')
        {
            /* Ask the user for the new name */
            char name[256];
            ask_for_name(name, 1);

            /* Update the doctor's name */
            strcpy(doctor->name, name);
        }
        else if (choice == '3')
        {

            /* Ask the user for the new email */
            char email[256];
            ask_for_email(email, 1);

            /* Update the doctor's email */
            strcpy(doctor->email, email);
        }
        else if (choice == '4')
        {

            /* Ask the user for the new phone */
            char phone[256];
            ask_for_phone(phone, 1);

            /* Update the doctor's phone */
            strcpy(doctor->phone, phone);
        }
        else if (choice == '5')
        {

            /* Ask the user for the new password */
            unsigned int password = ask_for_password(1);

            /* Update the doctor's password */
            doctor->password = password;
        }
        else if (choice == '6')
        {

            /* Ask the user for the new specialization */
            char specialization[256];
            read_string("Enter your new specialization: ", specialization,
                        sizeof(specialization));

            /* Update the doctor's specialization */
            strcpy(doctor->specialization, specialization);
        }
        else if (choice == '7')
        {

            /* Ask the user for the new license number */
            char license_number[256];
            read_string("Enter your new license number: ", license_number,
                        sizeof(license_number));

            /* Update the doctor's license number */
            strcpy(doctor->license_number, license_number);

        /* Otherwise, invalid option */
        } else {
            /* Invalid option */
            printf("Invalid choice\n");
        }

        /* Update the records after every action */
        save_database(records);
    }
}

/*******************************************************************************
 * View all patients in the hospital records.
 *
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 *******************************************************************************/
void view_all_patients(hospital_record_t *records)
{

    /* Get the patients from the hospital records */
    patient_details_t *patients = records->patients;

    /* Iterate through the patients */
    while (patients != NULL)
    {
        print_patient_details(patients);
        patients = patients->next;
    }
}

/*******************************************************************************
 * View a specific patient's details in the hospital records.
 *
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 *******************************************************************************/
void view_patient_details(hospital_record_t *records)
{

    /* Ask the user for the username of the patient to view */
    char username[256];
    read_string("Enter the username of the patient to view: ", username,
                sizeof(username));

    /* Find the patient in the hospital records */
    patient_details_t *patient = find_patient(records, username);

    /* If the patient is not found, print an error message */
    if (patient == NULL)
    {
        printf("Patient not found\n");
        return;
    }

    /* Print the patient details */
    print_patient_details(patient);
}

/*******************************************************************************
 * Updates a patient's details in the hospital records.
 *
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 *******************************************************************************/
void update_patient_details_doctor(hospital_record_t *records)
{
    /* Ask the user for the username of the patient to update */
    char username[256];
    read_string("Enter the username of the patient to update: ", username,
                sizeof(username));

    /* Find the patient in the hospital records */
    patient_details_t *patient = find_patient(records, username);

    /* If the patient is not found, print an error message */
    if (patient == NULL)
    {
        printf("Patient not found\n");
        return;
    }

    /* Update the patient's details */
    update_patient_details(records, patient);
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
           "6. Update your details\n"
           "7. Update a patient's details\n"
           "8. Assign a patient to a bed\n"
           "9. View all beds\n"
           "10. Discharge patient\n"
           "X. Exit\n");
}

/*******************************************************************************
 * Assigns a patient to a bed in the hospital records.
 *
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 *******************************************************************************/
void assign_patient_to_bed(hospital_record_t *records) {

    /* Check if bed capacity is full */
    if (records->num_beds == 0) {
        printf("No beds available\n");
        return;
    }

    /* Ask for the username of the patient to assign to a bed */
    char username[256];
    read_string("Enter the username of the patient to assign to a bed: ", username,
                sizeof(username));

    /* Find the patient in the hospital records */
    patient_details_t *patient = find_patient(records, username);

    /* If the patient is not found, print an error message */
    if (patient == NULL) {
        printf("Patient not found\n");
        return;
    }

    /* Look for an available bed */
    int i;
    for (i = 0; i < records->num_beds; i++) {
        
        /* Once a bed is found with no patient*/
        if (records->beds[i].patient == NULL) {

            /* Assign the patient to the bed */
            records->beds[i].patient = patient;
            records->num_beds_in_use += 1;
            break;
        }
    }

    /* Print a success message */
    printf("Patient assigned to bed %d\n", i + 1);
}

/*******************************************************************************
 * View all beds in the hospital records.
 *
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 *******************************************************************************/
void view_all_beds(hospital_record_t *records) {

    /* Iterate through the beds */
    int i;
    for (i = 0; i < records->num_beds; i++) {

        /* If the bed is empty, print that it is empty */
        if (records->beds[i].patient == NULL) {
            printf("Bed %d: Empty\n", i + 1);
            
        /* If the bed is not empty, print the patient's details */
        } else {
            printf("Bed %d: ", i + 1);
            print_patient_details(records->beds[i].patient);
        }
    }
}

/*******************************************************************************
 * Discharges a patient from a bed in the hospital records.
 *
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 ******************************************************************************/
void discharge_patient(hospital_record_t *records) {

    /* Ask for the username of the patient to discharge */
    char username[256];
    read_string("Enter the username of the patient to discharge: ", username,
                sizeof(username));

    /* Find the patient in the hospital records */
    patient_details_t *patient = find_patient(records, username);

    /* If the patient is not found, print an error message */
    if (patient == NULL) {
        printf("Patient not found\n");
        return;
    }

    /* Discharge the patient */
    records->beds[records->num_beds_in_use].patient = NULL;
    records->num_beds_in_use -= 1;

    /* Print a success message */
    printf("Patient discharged\n");
}

/*******************************************************************************
 * Entry point to use the 'doctor' menu.
 *
 * inputs:
 * - none
 * outputs:
 * - none
 *******************************************************************************/
void doctor_use(hospital_record_t *records, doctor_details_t *doctor)
{
    /* Print a welcome message to the doctor */
    printf("Welcome %s\n", doctor->name);

    /* Show the available choices */
    print_doctor_menu();

    /* Get the user's choice */
    /* 3 characters will store any choice + null terminator */
    char choice[3];
    while (read_string("Enter your choice: ", choice, sizeof(choice)) != 0)
    {
        /* Exit if requested */
        if (strcmp(choice, "X") == 0)
        {
            break;
        }

        /* Process the menu choice */
        if (strcmp(choice, "1") == 0) {
            patient_signup(records);
        } else if (strcmp(choice, "2") == 0) {
            view_all_patients(records);
        } else if (strcmp(choice, "3") == 0) {
            view_patient_details(records);
        } else if (strcmp(choice, "4") == 0) {
            delete_patient(records);
        } else if (strcmp(choice, "5") == 0) {
            doctor_signup(records);
        } else if (strcmp(choice, "6") == 0) {
            update_doctor_details(records, doctor);
        } else if (strcmp(choice, "7") == 0) {
            update_patient_details_doctor(records);
        } else if (strcmp(choice, "8") == 0) {
            assign_patient_to_bed(records);
        } else if (strcmp(choice, "9") == 0) {
            view_all_beds(records);
        } else if (strcmp(choice, "10") == 0) {

            printf("Todo\n");
        } else {
            printf("Invalid choice\n");
        }

        /* Update the records after every action */
        save_database(records);
    }

    /* Goodbye message */
    printf("Thank you for using the hospital records system\n");
}

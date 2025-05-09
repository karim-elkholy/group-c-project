#include <stdio.h>
#include <string.h>

#include "application/database.h"
#include "utils/scanner.h"
#include "utils/hash.h"


/*******************************************************************************
 * Silently adds a new patient to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * - patient - The patient to add
 * outputs:
 * - none
 ******************************************************************************/
void patient_signup_silent(
    hospital_record_t *records, 
    patient_details_t *patient
) {

    /* If this is the first patient */
    if ( records->num_patients == 0 ) {

        /* Add the patient to the list */
        records->patients = patient;

        /* Mark the end of the list */
        records->patients->next = NULL;
    
    /* If this is not the first entry */
    } else {

        /* Find the last patient*/
        patient_details_t *last_patient = records->patients;
        while (last_patient->next != NULL) {
            last_patient = last_patient->next;
        }

        /* Add the new patient to the end of the list */
        last_patient->next = patient;
    }

    /* Update the number of patients */
    records->num_patients += 1;
}

/*******************************************************************************
 * Adds a new patient to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 ******************************************************************************/
void patient_signup(hospital_record_t *records) {

    printf("--------------------------------\n");
    printf("Patient Signup \n");
    printf("--------------------------------\n");

    /* Ask the user for their details */
    char username[256];
    read_string("Username: ", username, sizeof(username));
    /* Name */
    char name[256];
    read_string("Name: ", name, sizeof(name));
    /* Email */
    char email[256];
    read_string("Email: ", email, sizeof(email));
    /* Phone */
    char phone[256];
    read_string("Phone: ", phone, sizeof(phone));
    /* Password */
    char password[256];
    read_string("Password: ", password, sizeof(password));
    /* Blood type */
    char blood_type[256];
    read_string("Blood type: ", blood_type, sizeof(blood_type));
    /* Medical history */
    char medical_history[256];
    read_string("Medical history: ", medical_history, sizeof(medical_history));
    /* Allergies */
    char allergies[256];
    read_string("Allergies: ", allergies, sizeof(allergies));
    /* Medications */
    char medications[256];
    read_string("Medications: ", medications, sizeof(medications));
    /* Weight */
    float weight;
    scanf("%f", &weight);
    /* Height */
    float height;
    scanf("%f", &height);
    /* Extra notes about the patient */
    char notes[256];
    read_string("Notes: ", notes, sizeof(notes));

    /* Create a new doctor */
    patient_details_t patient;
    /* Username */
    strcpy(patient.username, username);
    /* Name */
    strcpy(patient.name, name);
    /* Email */
    strcpy(patient.email, email);
    /* Phone */
    strcpy(patient.phone, phone);
    /* Password */
    patient.password = hash_string(password);
    /* Blood type */
    strcpy(patient.blood_type, blood_type);
    /* Medical history */
    strcpy(patient.medical_history, medical_history);
    /* Allergies */
    strcpy(patient.allergies, allergies);
    /* Medications */
    strcpy(patient.medications, medications);
    /* Weight */
    patient.weight = weight;
    /* Height */
    patient.height = height;
    /* Extra notes about the patient */
    strcpy(patient.notes, notes);

    /* Add the patient to the hospital records */
    records->patients[records->num_patients] = patient;
    records->num_patients += 1;

    /* Print a success message if signup is successful */
    printf("Signup successful\n");
}


/*******************************************************************************
 * Finds a patient in the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * - num_users - The number of users in the database
 * - user_id - The ID of the user to find
 * outputs:
 * - The user if found, otherwise NULL
 ******************************************************************************/
patient_details_t *find_patient(hospital_record_t *records, 
    char *user_id) {

    /* Get the patients from the hospital records */
    patient_details_t *patients = records->patients;

    /* Iterate through the patients */
    while ( patients != NULL) {

        /* If this user's ID matches the given ID */
        if (strcmp(patients->username, user_id) == 0) {
            return patients;
        }

        /* Move to the next patient */
        patients = patients->next;
    }

    /* If no patient is found, return NULL */
    return NULL;

}

/*******************************************************************************
 * Prints the patient details.
 * 
 * inputs:
 * - patient - The patient to print
 * outputs:
 * - none
 ******************************************************************************/
void print_patient_details(patient_details_t *patient) {

    /* Print the patient details */
    printf("Username: %s\n", patient->username);
    printf("Name: %s\n", patient->name);
    printf("Email: %s\n", patient->email);
    printf("Phone: %s\n", patient->phone);
    printf("Blood type: %s\n", patient->blood_type);
    printf("Medical history: %s\n", patient->medical_history);
    printf("Allergies: %s\n", patient->allergies);  
    printf("Medications: %s\n", patient->medications);
    printf("Weight: %f\n", patient->weight);
    printf("Height: %f\n", patient->height);
    printf("Notes: %s\n", patient->notes);
}

/*******************************************************************************
 * Prints the patient menu.
 * 
 * inputs:
 * - none
 * outputs:
 * - none
 *******************************************************************************/
void print_patient_menu() {

    printf("\n Patient Menu \n"
        "1. View records\n"
        "2. Update details\n"
        "3. Exit\n");
}

/*******************************************************************************
 * Entry point to use the 'patient' menu.
 * 
 * inputs:
 * - none
 * outputs:
 * - none
 *******************************************************************************/
void patient_use() {

    /* Login */

}
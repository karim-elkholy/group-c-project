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
 * - The newly created patient
 ******************************************************************************/
patient_details_t *patient_signup(hospital_record_t *records) {

    printf("--------------------------------\n");
    printf("Patient Signup \n");
    printf("--------------------------------\n");

    /* TODO - add form validation */
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
    char weight[256];
    read_string("Weight(kg): ", weight, sizeof(weight));
    float weight_float = atof(weight);
    /* Height */
    char height[256];
    read_string("Height(cm): ", height, sizeof(height));
    float height_float = atof(height);
    /* Extra notes about the patient */
    char notes[256];
    read_string("Notes: ", notes, sizeof(notes));

    /* Create a new doctor */
    patient_details_t *patient = malloc(sizeof(patient_details_t));
    /* Username */
    strcpy(patient->username, username);
    /* Name */
    strcpy(patient->name, name);
    /* Email */
    strcpy(patient->email, email);
    /* Phone */
    strcpy(patient->phone, phone);
    /* Password */
    patient->password = hash_string(password);
    /* Blood type */
    strcpy(patient->blood_type, blood_type);
    /* Medical history */
    strcpy(patient->medical_history, medical_history);
    /* Allergies */
    strcpy(patient->allergies, allergies);
    /* Medications */
    strcpy(patient->medications, medications);
    /* Weight */
    patient->weight = weight_float;
    /* Height */
    patient->height = height_float;
    /* Extra notes about the patient */
    strcpy(patient->notes, notes);

    /* Add the patient to the hospital records */
    patient_signup_silent(records, patient);

    /* Print a success message if signup is successful */
    printf("Signup successful\n");

    /* Return the newly created patient */
    return patient;
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
 * Prints the choices available for the patient update menu.
 *
 * inputs:
 * - none
 * outputs:
 * - none
 ******************************************************************************/
void print_patient_update_menu()
{
    printf("\n Patient Update Menu \n"
           "1. Username\n"
           "2. Name\n"
           "3. Email\n"
           "4. Phone\n"
           "5. Password\n"
           "6. Blood type\n"
           "7. Medical history\n"
           "8. Allergies\n"
           "9. Medications\n"
           "10. Weight\n"
           "11. Height\n"
           "12. Notes\n"
           "X. Exit\n");
}

/*******************************************************************************
 * Updates a patient's details in the hospital records.
 *
 * inputs:
 * - records - The hospital records
 * - patient - The patient to update
 *******************************************************************************/
void update_patient_details(
    hospital_record_t *records, patient_details_t *patient)
{

    /* Print the update menu */
    print_patient_update_menu();

    /* Get the user's choice */
    /* 3 characters will store any choice + null terminator */
    char choice[3];
    while (read_string("Enter your choice: ", choice, sizeof(choice)) != 0)
    {
        /* Exit if requested*/
        if (strcmp(choice, "X") == 0)
        {
            break;
        }

        /* Process the menu choice */
        if (strcmp(choice, "1") == 0) {
            char username[256];
            read_string("Enter your new username: ", username, sizeof(username));
            strcpy(patient->username, username);
        } else if (strcmp(choice, "2") == 0) {
            char name[256];
            read_string("Enter your new name: ", name, sizeof(name));
            strcpy(patient->name, name);
        } else if (strcmp(choice, "3") == 0) {
            char email[256];
            read_string("Enter your new email: ", email, sizeof(email));
            strcpy(patient->email, email);
        } else if (strcmp(choice, "4") == 0) {
            char phone[256];
            read_string("Enter your new phone: ", phone, sizeof(phone));
            strcpy(patient->phone, phone);
        } else if (strcmp(choice, "5") == 0) {
            char password[256];
            read_string("Enter your new password: ", password, sizeof(password));
            patient->password = hash_string(password);
        } else if (strcmp(choice, "6") == 0) {
            char blood_type[256];
            read_string("Enter your new blood type: ", 
                blood_type, sizeof(blood_type));
            strcpy(patient->blood_type, blood_type);
        } else if (strcmp(choice, "7") == 0) {
            char medical_history[256];
            read_string("Enter your new medical history: ", 
            medical_history, sizeof(medical_history));
            strcpy(patient->medical_history, medical_history);
        } else if (strcmp(choice, "8") == 0) {
            char allergies[256];
            read_string("Enter your new allergies: ", 
                allergies, sizeof(allergies));
            strcpy(patient->allergies, allergies);
        } else if (strcmp(choice, "9") == 0) {
            char medications[256];
            read_string("Enter your new medications: ",
                medications, sizeof(medications));
            strcpy(patient->medications, medications);
        } else if (strcmp(choice, "10") == 0) {
            float weight;
            printf("Enter your new weight: ");
            scanf("%f", &weight);
            patient->weight = weight;
        } else if (strcmp(choice, "11") == 0) {
            float height;
            printf("Enter your new height: ");
            scanf("%f", &height);
            patient->height = height;
        } else if (strcmp(choice, "12") == 0) {
            char notes[256];
            read_string("Enter your new notes: ", notes, sizeof(notes));
            strcpy(patient->notes, notes);
        } else {
            printf("Invalid choice\n");
        }

        /* Update the records after every action */
        save_database(records);
    }
}

/*******************************************************************************
 * Deletes a patient from the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * - username - The username of the patient to delete
 * outputs:
 * - none
 ******************************************************************************/
void delete_patient_silent(hospital_record_t *records, char *username) {

    /* Get the patients from the hospital records */
    patient_details_t *patients = records->patients;

    /* Stores the previous patient */
    patient_details_t *prev_patient = NULL;

    /* Iterate through the patients */
    while ( patients != NULL) {

        /* If this user's ID matches the given ID */
        if (strcmp(patients->username, username) == 0) {

            /* Get the current patient */
            patient_details_t *current_patient = patients;

            /* If there was a previous patient */
            if (prev_patient != NULL) {

                /* Update the next pointer of the previous patient */
                prev_patient->next = patients->next;

            /* Otherwise if this is the first patient */
            } else {

                /* Update the head of the list */
                records->patients = patients->next;
            }

            /* Free the memory allocated for the current patient */
            free(current_patient);

            /* Decrement the number of patients */
            records->num_patients -= 1;

            /* Return */
            return;
        }

        /* Move to the next patient */
        patients = patients->next;
    }

}

/*******************************************************************************
 * Deletes a patient from the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 ******************************************************************************/
void delete_patient(hospital_record_t *records) {

    /* Ask the user for the username of the patient to delete */
    char username[256];
    read_string("Enter the username of the patient to delete: ", username, 
        sizeof(username));

    /* Delete the patient from the hospital records */
    delete_patient_silent(records, username);

    /* Print a success message if deletion is successful */
    printf("Patient deleted successfully\n");
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

    /* Stylish header */
    printf("--------------------------------\n");
    printf("Patient Menu \n");
    printf("--------------------------------\n");

    printf("1. View records\n"
           "2. Update details\n"
           "3. Exit\n");
}

/*******************************************************************************
 * Entry point to use the 'patient' menu.
 * 
 * inputs:
 * - records - The hospital records
 * - patient - The patient to use
 * outputs:
 * - none
 *******************************************************************************/
void patient_use(hospital_record_t *records, patient_details_t *patient) {

    /* Print a welcome message to the doctor */
    printf("Welcome %s\n", patient->name);

    /* Show the available choices */
    print_patient_menu();

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
            print_patient_details(patient);
        } else if (strcmp(choice, "2") == 0) {
            update_patient_details(records, patient);
        } else {
            printf("Invalid choice\n");
        }

        /* Update the records after every action */
        save_database(records);
    }

    /* Goodbye message */
    printf("Thank you for using the hospital records system\n");
}
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "application/database.h"
#include "utils/scanner.h"
#include "utils/hash.h"
#include "utils/input.h"

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
 * Calculates the BMI of a patient.
 *
 * inputs:
 * - weight - The weight of the patient
 * - height - The height of the patient
 * outputs:
 * - The BMI of the patient
 ******************************************************************************/
float calculate_bmi(float weight, float height) {
    /* Convert the height to meters */
    float height_meters = height / 100;

    /* Calculate the BMI */
    return weight / (pow(height_meters, 2));
}

/*******************************************************************************
 * Silently adds a new patient to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * - patient - The patient to add
 * outputs:
 * - none
 ******************************************************************************/
void patient_signup_batch(
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

    /* Indicate a patient has been successfully added */
    printf("Patient %s added successfully\n", patient->username);
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
int patient_validate_username(hospital_record_t *records, char *username) {

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

    /* If another patient has this username */
    if (find_patient(records, username) != NULL) {
        printf("Patient %s already exists\n", username);
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
void patient_ask_for_username(
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
        if (patient_validate_username(records, username) == 0) {
            break;
        }
    }
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
    patient_ask_for_username(records, username, 0);
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
    /* Blood type */
    char blood_type[256];
    ask_for_blood_type(blood_type, 0);
    /* Medical history */
    char medical_history[256];
    ask_for_medical_history(NULL, medical_history);
    /* Weight */
    float weight_float = ask_for_weight(0);
    /* Height */
    float height_float = ask_for_height(0);

    /* Create a new patient */
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
    patient->password = password;
    /* Blood type */
    strcpy(patient->blood_type, blood_type);
    /* Medical history */
    strcpy(patient->medical_history, medical_history);
    /* Weight */
    patient->weight = weight_float;
    /* Height */
    patient->height = height_float;
    /* BMI */
    patient->bmi = calculate_bmi(patient->weight, patient->height);

    /* Add the patient to the hospital records */
    patient_signup_silent(records, patient);

    /* Print a success message */
    printf("Patient %s added successfully\n", patient->username);

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
           "7. Add to Medical history\n"
           "8. Clear Medical history\n"
           "9. Weight\n"
           "10. Height\n"
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
            /* Ask for the new username */
            char username[256];
            patient_ask_for_username(records, username, 1);
            /* Update the username */
            strcpy(patient->username, username);
        } else if (strcmp(choice, "2") == 0) {
            ask_for_name(patient->name, 1);
        } else if (strcmp(choice, "3") == 0) {
            ask_for_email(patient->email, 1);
        } else if (strcmp(choice, "4") == 0) {
            ask_for_phone(patient->phone, 1);
        } else if (strcmp(choice, "5") == 0) {
            patient->password = ask_for_password(1);
        } else if (strcmp(choice, "6") == 0) {
            ask_for_blood_type(patient->blood_type, 1);
        } else if (strcmp(choice, "7") == 0) {
            ask_for_medical_history(patient->medical_history, patient->medical_history);
        } else if (strcmp(choice, "8") == 0) {
            patient->medical_history[0] = '\0';
        } else if (strcmp(choice, "9") == 0) {
            patient->weight = ask_for_weight(1);

            /* Update BMI */
            patient->bmi = calculate_bmi(patient->weight, patient->height);
        } else if (strcmp(choice, "10") == 0) {
            patient->height = ask_for_height(1);

            /* Update BMI */
            patient->bmi = calculate_bmi(patient->weight, patient->height);
        } else {
            printf("Invalid choice\n");
        }

        /* Update the records after every action */
        save_database(records);

        /* Print the menu again */
        print_patient_update_menu();
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

    /* TODO - Jordan ASCII ART */
    printf("--------------------------------\n");

    /* Print the patient details */
    printf("Username: %s\n", patient->username);
    printf("Name: %s\n", patient->name);
    printf("Email: %s\n", patient->email);
    printf("Phone: %s\n", patient->phone);
    printf("Blood type: %s\n", patient->blood_type);
    printf("Medical history: %s\n", patient->medical_history);
    printf("Weight: %.2fkg\n", patient->weight);
    printf("Height: %.2fcm\n", patient->height);
    printf("BMI: %f\n", patient->bmi);
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

    /* taken from https://www.oocities.org/spunk1111/small.htm */
    printf("                 _.-;;-._ \n");
    printf("          '-..-'|   ||   |\n");
    printf("          '-..-'|_.-;;-._|\n");
    printf("          '-..-'|   ||   |\n");
    printf("          '-..-'|_.-''-._|\n");
    
    printf("1. View records\n"
           "2. Update details\n"
           "X. Exit\n");
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

        /* Print the menu again */
        print_patient_menu();
    }

    /* Goodbye message */
    printf("Thank you for using the hospital records system\n");
}
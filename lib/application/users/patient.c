#include <stdio.h>
#include <string.h>

#include "application/database.h"

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
 * Finds a patient in the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * - num_users - The number of users in the database
 * - user_id - The ID of the user to find
 * outputs:
 * - The user if found, otherwise NULL
 ******************************************************************************/
patient_details_t *find_patient(patient_details_t **records, int patient_count, char *user_id) {

    /* Iterate through the patients */
    int i;
    for (i = 0; i < patient_count; i++) {

        /* If this user's ID matches the given ID */
        if (strcmp(records[i]->id, user_id) == 0) {
            return records[i];
        }
    }

    /* If no doctor is found, return NULL */
    return NULL;
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
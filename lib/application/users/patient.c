#include <stdio.h>
#include <application/users.h>
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
user_record_t *find_patient(user_record_t *records, char *user_id) {

    /* Iterate through the doctors */
    int i;
    for (i = 0; i < records->num_doctors; i++) {

        /* If this user's ID matches the given ID */
        if (strcmp(records->doctors[i].id, user_id) == 0) {
            return &records->doctors[i];
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
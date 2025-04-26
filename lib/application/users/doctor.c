#include <stdio.h>
#include <application/users.h>

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
user_record_t *find_doctor(user_record_t *records, char *user_id) {

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
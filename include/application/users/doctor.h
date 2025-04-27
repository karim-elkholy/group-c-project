#ifndef APPLICATION_USERS_DOCTOR_H
#define APPLICATION_USERS_DOCTOR_H


#include "application/users/user.h"

/* Doctor details */
typedef struct doctor_details {

    /* User details */
    user_t user;

    /* Specialization */
    char specialization[256];
    
    /* License number */
    char license_number[256];
} doctor_details_t;

/*Forward declaration*/
typedef struct record record_t;

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
doctor_details_t *find_doctor(record_t *records, char *id);

/*******************************************************************************
 * Handles access to the 'doctor' menu.
 * Ensures user is authenticated before providing access. 
 * 
 * inputs:
 * - records - The database of users.
 * outputs:
 * - The doctor if found, otherwise NULL
 ******************************************************************************/
void doctor_login(record_t *records);

/*******************************************************************************
 * Entry point to use the 'doctor' menu.
 * 
 * inputs:
 * - records - The records of the hospital.
 * outputs:
 * - none
 ******************************************************************************/
void doctor_use(doctor_details_t *doctor);

#endif


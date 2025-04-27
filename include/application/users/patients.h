#ifndef APPLICATION_USERS_PATIENTS_H
#define APPLICATION_USERS_PATIENTS_H

#include "application/users/user.h"

/* Patient details */
typedef struct patient_details {
    /* User details */
    user_t user;

    /* Blood type */
    char blood_type[3];
    /* Medical history */
    char medical_history[256];
    /* Allergies */
    char allergies[256];
    /* Medications */
    char medications[256];

    /* Weight */
    float weight;

    /* Height */
    float height;

    /* Extra notes about the patient */
    char notes[256];
} patient_details_t;

/* Forward declaration */
typedef struct record record_t;

/*******************************************************************************
 * Entry point to use the 'patient' menu.
 * 
 * inputs:
 * - records - The records of the hospital.
 * outputs:
 * - none
 ******************************************************************************/
void patient_use(patient_details_t *patient);

#endif



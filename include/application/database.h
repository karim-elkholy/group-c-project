#ifndef APPLICATION_DATABASE_H
#define APPLICATION_DATABASE_H

#include "application/users/patient.h"
#include "application/users/doctor.h"

/* Holds the hospital records */
struct hospital_record {
    /* Patients */
    patient_details_t *patients;
    /* Doctors */
    doctor_details_t *doctors;

    /* Number of patients */
    int num_patients;
    /* Number of doctors */
    int num_doctors;


    /* Number of available slots for patients */
    int num_patients_available;
    /* Number of available slots for doctors */
    int num_doctors_available;
};

typedef struct hospital_record hospital_record_t;

/*******************************************************************************
 * Load the database.
 * 
 * inputs:
 * - hospital_name - The name of the hospital.
 * - num_users - The number of users in the database.
 * outputs:
 * - The database.
 ******************************************************************************/
hospital_record_t *load_database(const char *hospital_name, int *num_users);

/*******************************************************************************
 * Save the database.
 * 
 * inputs:
 * - hospital_name - The name of the hospital.
 * - records - The database.
 ******************************************************************************/
void save_database(const char *hospital_name, hospital_record_t *records);

/*******************************************************************************
 * Close the database.
 * Should be called before closing to free memory allocated for the database.
 * 
 * inputs:
 * - records - The database.
 ******************************************************************************/
void close_database(hospital_record_t *records);

#endif

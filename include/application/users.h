#ifndef APPLICATION_USERS_H
#define APPLICATION_USERS_H

#include "application/users/patient.h"
#include "application/users/doctor.h"

/* Holds the records for all the different users */
struct user_record {
    /* Patients */
    patient_details_t *patients;
    /* Doctors */
    doctor_details_t *doctors;
    /* Number of patients */
    int num_patients;
    /* Number of doctors */
    int num_doctors;
};

typedef struct user_record user_record_t;
#endif


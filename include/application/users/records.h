#ifndef APPLICATION_USERS_RECORDS_H
#define APPLICATION_USERS_RECORDS_H

#include "application/users/patients.h"
#include "application/users/doctor.h"

typedef struct record {
    /* Number of patients */
    int num_patients;

    /* Number of doctors */
    int num_doctors;

    /* Patients */
    patient_details_t patients[256];

    /* Doctors */
    doctor_details_t doctors[256];
    
} record_t;




#endif

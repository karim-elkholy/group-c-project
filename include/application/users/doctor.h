#ifndef APPLICATION_USERS_DOCTOR_H
#define APPLICATION_USERS_DOCTOR_H


struct doctor_details {
    /* ID */
    char id[256];
    
    /* Name */
    char name[256];
    
    /* Email */
    char email[256];

    /* Phone */
    char phone[256];

    /* Hashed password */
    unsigned int password;

    /* Specialization */
    char specialization[256];
    
    /* License number */
    char license_number[256];
};

typedef struct doctor_details doctor_details_t;

/*******************************************************************************
 * Adds a new doctor to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 *******************************************************************************/
void doctor_signup(hospital_record_t *records);

/* Finds a doctor by their ID.*/
doctor_details_t *find_doctor(doctor_details_t *records, int doctor_count, char *user_id);

/* Entry point for the doctor menu.*/
void doctor_menu(doctor_details_t *doctor);

#endif


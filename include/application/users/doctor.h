#ifndef APPLICATION_USERS_DOCTOR_H
#define APPLICATION_USERS_DOCTOR_H

typedef struct hospital_record hospital_record_t;

struct doctor_details {
    /* ID */
    char username[256];
    
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

    /* Next doctor */
    /* Needed for linked list */
    struct doctor_details *next;
};

typedef struct doctor_details doctor_details_t;
/*******************************************************************************
 * Silently adds a new doctor to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * - doctor - The doctor to add
 * outputs:
 * - none
 ******************************************************************************/
void doctor_signup_silent(hospital_record_t *records, doctor_details_t *doctor);

/*******************************************************************************
 * Interactively adds a new doctor to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * outputs:
 * - none
 ******************************************************************************/
void doctor_signup(hospital_record_t *records);

/*******************************************************************************
 * Finds a doctor by their ID.
 * 
 * inputs:
 * - records - The hospital records
 * - user_id - The ID of the doctor to find
 * outputs:
 * - The doctor if found, otherwise NULL
 ******************************************************************************/
doctor_details_t *find_doctor(hospital_record_t *records, char *user_id);

/*******************************************************************************
 * Entry point for the doctor menu.
 * 
 * inputs:
 * - records - The hospital records
 * - doctor - The doctor to use
 * outputs:
 * - none
 ******************************************************************************/
void doctor_use(hospital_record_t *records, doctor_details_t *doctor);

#endif


#ifndef APPLICATION_USERS_PATIENT_H
#define APPLICATION_USERS_PATIENT_H

typedef struct hospital_record hospital_record_t;

struct patient_details {

    /* Username */
    char username[256];
    
    /* Name */
    char name[256];
    
    /* Email */
    char email[256];

    /* Phone */
    char phone[256];

    /* Hashed password */
    unsigned int password;

    /* Blood type */
    char blood_type[3];
    /* Medical history(includes allergies and medications) */
    char medical_history[256];
    /* Weight */
    float weight;
    /* BMI */
    float bmi;
    
    /* Height */
    float height;

    /* Next patient */
    /* Needed for linked list */
    struct patient_details *next;
};

typedef struct patient_details patient_details_t;

/*******************************************************************************
 * Silently adds a new patient to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * - patient - The patient to add
 * outputs:
 * - none
 ******************************************************************************/
void patient_signup_silent(
    hospital_record_t *records, 
    patient_details_t *patient
);

/*******************************************************************************
 * Adds a new patient to the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 * outputs:
 * - The newly created patient
 ******************************************************************************/
patient_details_t *patient_signup(hospital_record_t *records);

/*******************************************************************************
 * Updates the patient details.
 * 
 * inputs:
 * - records - The hospital records
 * - patient - The patient to update
 ******************************************************************************/
void update_patient_details(
    hospital_record_t *records, 
    patient_details_t *patient
);

/*******************************************************************************
 * Prints the patient details.
 * 
 * inputs:
 * - patient - The patient to print
 * outputs:
 * - none
 ******************************************************************************/
void print_patient_details(patient_details_t *patient);

/*******************************************************************************
 * Finds a patient by their ID.
 * 
 * inputs:
 * - records - The hospital records
 * - user_id - The ID of the patient to find
 * outputs:
 * - The patient if found, otherwise NULL
 ******************************************************************************/
patient_details_t *find_patient(hospital_record_t *records, char *user_id);

/*******************************************************************************
 * Deletes a patient from the hospital records.
 * 
 * inputs:
 * - records - The hospital records
 ******************************************************************************/
void delete_patient(hospital_record_t *records);

/*******************************************************************************
 * Entry point to use the 'patient' menu.
 * 
 * inputs:
 * - records - The hospital records
 * - patient - The patient to use
 * outputs:
 * - none
 *******************************************************************************/
void patient_use(hospital_record_t *records, patient_details_t *patient);
#endif


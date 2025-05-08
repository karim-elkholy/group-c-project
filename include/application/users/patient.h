#ifndef APPLICATION_USERS_PATIENT_H
#define APPLICATION_USERS_PATIENT_H


struct patient_details {

    /* ID */
    char id[256];
    
    /* Name */
    char name[256];
    
    /* Email */
    char email[256];

    /* Phone */
    char phone[256];

    /* Hashed password */
    char password[256];

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
};

typedef struct patient_details patient_details_t;

/* Adds a new patient by writing to the patient array.*/
void addPatient(patient_details_t *patients, int *num_patients);

/* Frees memory allocated for a single patient.*/
void delete_patient(patient_details_t *patient);

/* Displays all patients in the array (for viewing purposes).*/
void display_patients(patient_details_t *patients[], int num_patients);

#endif


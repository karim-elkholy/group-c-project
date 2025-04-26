#ifndef APPLICATION_USERS_H
#define APPLICATION_USERS_H

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

/* Entry point to use the 'patient' menu */
void patient_use();

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
    char password[256];

    /* Specialization */
    char specialization[256];
    
    /* License number */
    char license_number[256];
};

typedef struct doctor_details doctor_details_t;

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


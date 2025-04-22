#ifndef USER_CORE_H
#define USER_CORE_H

struct patient {
    char *name;
    char *address;
    char *phone;
    char *email;
    char *password;

    // Blood type
    char *blood_type;

    // Date of birth
    char *date_of_birth;

    // Gender
    char *gender;

    // Weight
    float weight;

    // Allergies 
    char **allergies;

    // Medications 
    char **medications;

    
};

typedef struct patient patient_t;

struct doctor {
    char *name;
    char *address;
    char *phone;
    char *email;
    char *password;

    char *specialization;
};

typedef struct doctor doctor_t;

void addPatient(patient_t *patients, int *num_patients);

void addDoctor(doctor_t *doctors, int *num_doctors);

void delete_patient(patient_t *patient);

void delete_doctor(doctor_t *doctor);

void display_patients(patient_t *patients[], int num_patients);

void display_doctors(doctor_t *doctors[], int num_doctors);

void save_data(patient_t *patients[], int num_patients, doctor_t *doctors[], int num_doctors);

void load_data(patient_t *patients[], int num_patients, doctor_t *doctors[], int num_doctors);

#endif


#ifndef USER_CORE_H
#define USER_CORE_H

/* Struct to represent a patient record.*/
struct patient {
    char *name;
    char *address;
    char *phone;
    char *email;
    char *password;
    char *blood_type;
    char *date_of_birth;
    char *gender;
    float weight;
    char **allergies;       // Currently unused but reserved for expansion
    char **medications;     // Currently unused but reserved for expansion
};

typedef struct patient patient_t;

/* Struct to represent a doctor record*/
struct doctor {
    char *name;
    char *address;
    char *phone;
    char *email;
    char *password;
    char *specialization;
};
void read_string(const char *prompt, char *input, int input_size);
char read_choice(const char *prompt);

typedef struct doctor doctor_t;

/* Adds a new patient by writing to the patient array.*/
void addPatient(patient_t *patients, int *num_patients);

/* Adds a new doctor to the doctor array.*/
void addDoctor(doctor_t *doctors, int *num_doctors);

/* Frees memory allocated for a single patient.*/
void delete_patient(patient_t *patient);

/* Frees memory allocated for a single doctor.*/
void delete_doctor(doctor_t *doctor);

/* Displays all patients in the array (for viewing purposes).*/
void display_patients(patient_t *patients[], int num_patients);

/* Displays all doctors in the array (for viewing purposes).*/
void display_doctors(doctor_t *doctors[], int num_doctors);

/* Saves all patient and doctor data to a file.*/
void save_data(patient_t *patients[], int num_patients, doctor_t *doctors[], int num_doctors);

/* Loads patient and doctor data from a file.*/
void load_data(patient_t *patients[], int num_patients, doctor_t *doctors[], int num_doctors);

#endif


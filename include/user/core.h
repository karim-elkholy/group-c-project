/*******************************************************************************
 * File: core.h
 * Description: Header file containing data structures and function declarations
 *              for managing patients and doctors.
 * Author: Keiron Lee
 ******************************************************************************/

#ifndef USER_CORE_H
#define USER_CORE_H

/*******************************************************************************
 * Data Structures
 ******************************************************************************/

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
    char **allergies;
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

/*******************************************************************************
 * Patient/Doctor Add Functions
 * Author: Keiron Lee
 ******************************************************************************/

void addPatient(patient_t *patients, int *num_patients);
void addDoctor(doctor_t *doctors, int *num_doctors);

/*******************************************************************************
 * Memory Cleanup Functions
 * Author: Keiron Lee
 ******************************************************************************/

void delete_patient(patient_t *patient);
void delete_doctor(doctor_t *doctor);

/*******************************************************************************
 * Display Functions
 * Author: Keiron Lee
 ******************************************************************************/

void display_patients(patient_t *patients[], int num_patients);
void display_doctors(doctor_t *doctors[], int num_doctors);

/*******************************************************************************
 * File I/O Functions
 * Author: Keiron Lee
 ******************************************************************************/

void save_data(patient_t *patients[], int num_patients, doctor_t *doctors[], int num_doctors);
void load_data(patient_t *patients[], int num_patients, doctor_t *doctors[], int num_doctors);

#endif

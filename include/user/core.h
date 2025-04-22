/*
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
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_input.h"

patient_t *ask_patient_details() {
    patient_t *new_patient = (patient_t *)malloc(sizeof(patient_t));
    char buffer[256];

    printf("Enter patient name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    new_patient->name = strdup(buffer);

    printf("Enter patient address: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    new_patient->address = strdup(buffer);

    printf("Enter phone number: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_phone_number(buffer)) {
        printf("Invalid. Enter a valid phone number: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    new_patient->phone = strdup(buffer);

    printf("Enter email: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_email(buffer)) {
        printf("Invalid. Enter a valid email: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    new_patient->email = strdup(buffer);

    printf("Enter password: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    new_patient->password = strdup(buffer);

    printf("Enter blood type (A, B, AB, O): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_blood_type(buffer)) {
        printf("Invalid. Enter a valid blood type: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    new_patient->blood_type = strdup(buffer);

    printf("Enter date of birth (DD/MM/YYYY): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    new_patient->date_of_birth = strdup(buffer);

    printf("Enter gender (Male, Female, Other): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_gender(buffer)) {
        printf("Invalid. Enter valid gender: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    new_patient->gender = strdup(buffer);

    printf("Enter weight (kg): ");
    fgets(buffer, sizeof(buffer), stdin);
    new_patient->weight = atof(buffer);

    new_patient->allergies = NULL;
    new_patient->medications = NULL;

    return new_patient;
}

doctor_t *ask_doctor_details() {
    doctor_t *new_doctor = (doctor_t *)malloc(sizeof(doctor_t));
    char buffer[256];

    printf("Enter doctor name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    new_doctor->name = strdup(buffer);

    printf("Enter address: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    new_doctor->address = strdup(buffer);

    printf("Enter phone number: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_phone_number(buffer)) {
        printf("Invalid. Enter a valid phone number: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    new_doctor->phone = strdup(buffer);

    printf("Enter email: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_email(buffer)) {
        printf("Invalid. Enter a valid email: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    new_doctor->email = strdup(buffer);

    printf("Enter password: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    new_doctor->password = strdup(buffer);

    printf("Enter specialization: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    new_doctor->specialization = strdup(buffer);

    return new_doctor;
}

int is_valid_blood_type(char *blood_type) {
    return strcmp(blood_type, "A") == 0 ||
           strcmp(blood_type, "B") == 0 ||
           strcmp(blood_type, "AB") == 0 ||
           strcmp(blood_type, "O") == 0;
}

int is_valid_gender(char *gender) {
    return strcmp(gender, "Male") == 0 ||
           strcmp(gender, "Female") == 0 ||
           strcmp(gender, "Other") == 0;
}

int is_valid_email(char *email) {
    return strchr(email, '@') != NULL && strchr(email, '.') != NULL;
}

int is_valid_phone_number(char *phone_number) {
    int i;
    for (i = 0; phone_number[i] != '\0'; i++) {
        if (phone_number[i] < '0' || phone_number[i] > '9') {
            return 0;
        }
    }
    return i >= 8;
}


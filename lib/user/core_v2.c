/*******************************************************************************
 * File: core.c
 * Description: Core data logic for patient and doctor management system.
 * Author: Keiron Lee
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

/*******************************************************************************
 * Function: strdup_c90
 * --------------------
 * ANSI C-compatible replacement for strdup().
 * Allocates and copies a string.
 * Author: Keiron Lee
 ******************************************************************************/
char *strdup_c90(const char *s) {
    char *copy = (char *)malloc(strlen(s) + 1);
    if (copy != NULL) {
        strcpy(copy, s);
    }
    return copy;
}

/*******************************************************************************
 * Function: addPatient
 * --------------------
 * Empty placeholder for compatibility with original function declarations.
 * Author: Keiron Lee
 ******************************************************************************/
void addPatient(patient_t *patients, int *num_patients) {
}

/*******************************************************************************
 * Function: addDoctor
 * -------------------
 * Empty placeholder for compatibility with original function declarations.
 * Author: Keiron Lee
 ******************************************************************************/
void addDoctor(doctor_t *doctors, int *num_doctors) {
}

/*******************************************************************************
 * Function: delete_patient
 * ------------------------
 * Frees all dynamically allocated memory for a patient.
 * Author: Keiron Lee
 ******************************************************************************/
void delete_patient(patient_t *patient) {
    if (patient != NULL) {
        free(patient->name);
        free(patient->address);
        free(patient->phone);
        free(patient->email);
        free(patient->password);
        free(patient->blood_type);
        free(patient->date_of_birth);
        free(patient->gender);
        free(patient->allergies);
        free(patient->medications);
        free(patient);
    }
}

/*******************************************************************************
 * Function: delete_doctor
 * ------------------------
 * Frees all dynamically allocated memory for a doctor.
 * Author: Keiron Lee
 ******************************************************************************/
void delete_doctor(doctor_t *doctor) {
    if (doctor != NULL) {
        free(doctor->name);
        free(doctor->address);
        free(doctor->phone);
        free(doctor->email);
        free(doctor->password);
        free(doctor->specialization);
        free(doctor);
    }
}

/*******************************************************************************
 * Function: display_patients
 * --------------------------
 * Prints details of all patients in a readable format.
 * Author: Keiron Lee
 ******************************************************************************/
void display_patients(patient_t *patients[], int num_patients) {
    int i;
    for (i = 0; i < num_patients; i++) {
        printf("\nPatient #%d:\n", i + 1);
        printf("Name: %s\n", patients[i]->name);
        printf("Address: %s\n", patients[i]->address);
        printf("Phone: %s\n", patients[i]->phone);
        printf("Email: %s\n", patients[i]->email);
        printf("Blood Type: %s\n", patients[i]->blood_type);
        printf("Date of Birth: %s\n", patients[i]->date_of_birth);
        printf("Gender: %s\n", patients[i]->gender);
        printf("Weight: %.2f kg\n", patients[i]->weight);
    }
}

/*******************************************************************************
 * Function: display_doctors
 * -------------------------
 * Prints details of all doctors in a readable format.
 * Author: Keiron Lee
 ******************************************************************************/
void display_doctors(doctor_t *doctors[], int num_doctors) {
    int i;
    for (i = 0; i < num_doctors; i++) {
        printf("\nDoctor #%d:\n", i + 1);
        printf("Name: %s\n", doctors[i]->name);
        printf("Address: %s\n", doctors[i]->address);
        printf("Phone: %s\n", doctors[i]->phone);
        printf("Email: %s\n", doctors[i]->email);
        printf("Specialization: %s\n", doctors[i]->specialization);
    }
}

/*******************************************************************************
 * Function: save_data
 * -------------------
 * Saves patient and doctor data to a text file in a delimited format.
 * Author: Keiron Lee
 ******************************************************************************/
void save_data(patient_t *patients[], int num_patients, doctor_t *doctors[], int num_doctors) {
    FILE *fp = fopen("hospital_data.txt", "w");
    int i;

    if (fp == NULL) return;

    fprintf(fp, "PATIENTS %d\n", num_patients);
    for (i = 0; i < num_patients; i++) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%s|%s|%.2f\n",
            patients[i]->name,
            patients[i]->address,
            patients[i]->phone,
            patients[i]->email,
            patients[i]->password,
            patients[i]->blood_type,
            patients[i]->date_of_birth,
            patients[i]->gender,
            patients[i]->weight);
    }

    fprintf(fp, "DOCTORS %d\n", num_doctors);
    for (i = 0; i < num_doctors; i++) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s\n",
            doctors[i]->name,
            doctors[i]->address,
            doctors[i]->phone,
            doctors[i]->email,
            doctors[i]->password,
            doctors[i]->specialization);
    }

    fclose(fp);
}

/*******************************************************************************
 * Function: load_data
 * -------------------
 * Loads patient and doctor data from a text file and populates memory.
 * Author: Keiron Lee
 ******************************************************************************/
void load_data(patient_t *patients[], int num_patients, doctor_t *doctors[], int num_doctors) {
    FILE *fp = fopen("hospital_data.txt", "r");
    int i, count;
    char buffer[512];

    if (fp == NULL) return;

    fscanf(fp, "PATIENTS %d\n", &count);
    for (i = 0; i < count && i < num_patients; i++) {
        fgets(buffer, sizeof(buffer), fp);
        buffer[strcspn(buffer, "\n")] = '\0';

        patients[i] = (patient_t *)malloc(sizeof(patient_t));
        patients[i]->name = strdup_c90(strtok(buffer, "|"));
        patients[i]->address = strdup_c90(strtok(NULL, "|"));
        patients[i]->phone = strdup_c90(strtok(NULL, "|"));
        patients[i]->email = strdup_c90(strtok(NULL, "|"));
        patients[i]->password = strdup_c90(strtok(NULL, "|"));
        patients[i]->blood_type = strdup_c90(strtok(NULL, "|"));
        patients[i]->date_of_birth = strdup_c90(strtok(NULL, "|"));
        patients[i]->gender = strdup_c90(strtok(NULL, "|"));
        patients[i]->weight = (float)atof(strtok(NULL, "|"));
    }

    fscanf(fp, "DOCTORS %d\n", &count);
    for (i = 0; i < count && i < num_doctors; i++) {
        fgets(buffer, sizeof(buffer), fp);
        buffer[strcspn(buffer, "\n")] = '\0';

        doctors[i] = (doctor_t *)malloc(sizeof(doctor_t));
        doctors[i]->name = strdup_c90(strtok(buffer, "|"));
        doctors[i]->address = strdup_c90(strtok(NULL, "|"));
        doctors[i]->phone = strdup_c90(strtok(NULL, "|"));
        doctors[i]->email = strdup_c90(strtok(NULL, "|"));
        doctors[i]->password = strdup_c90(strtok(NULL, "|"));
        doctors[i]->specialization = strdup_c90(strtok(NULL, "|"));
    }

    fclose(fp);
}

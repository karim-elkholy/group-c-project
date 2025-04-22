#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

void addPatient(patient_t *patients, int *num_patients) {
}

void addDoctor(doctor_t *doctors, int *num_doctors) {
}

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
        patients[i]->name = strdup(strtok(buffer, "|"));
        patients[i]->address = strdup(strtok(NULL, "|"));
        patients[i]->phone = strdup(strtok(NULL, "|"));
        patients[i]->email = strdup(strtok(NULL, "|"));
        patients[i]->password = strdup(strtok(NULL, "|"));
        patients[i]->blood_type = strdup(strtok(NULL, "|"));
        patients[i]->date_of_birth = strdup(strtok(NULL, "|"));
        patients[i]->gender = strdup(strtok(NULL, "|"));
        patients[i]->weight = (float)atof(strtok(NULL, "|"));
    }

    fscanf(fp, "DOCTORS %d\n", &count);
    for (i = 0; i < count && i < num_doctors; i++) {
        fgets(buffer, sizeof(buffer), fp);
        buffer[strcspn(buffer, "\n")] = '\0';

        doctors[i] = (doctor_t *)malloc(sizeof(doctor_t));
        doctors[i]->name = strdup(strtok(buffer, "|"));
        doctors[i]->address = strdup(strtok(NULL, "|"));
        doctors[i]->phone = strdup(strtok(NULL, "|"));
        doctors[i]->email = strdup(strtok(NULL, "|"));
        doctors[i]->password = strdup(strtok(NULL, "|"));
        doctors[i]->specialization = strdup(strtok(NULL, "|"));
    }

    fclose(fp);
}

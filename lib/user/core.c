
#include "user/core.h"
/* Functions used: malloc, free */
#include <stdlib.h>

/* Functions used: printf, fopen, fclose, fprintf, fscanf */
#include <stdio.h>

/* Functions used: strlen, strcpy, strncpy, sprintf */
#include <string.h>

#define MAX_INPUT 100 // Maximum buffer size for input

// Reads user input from stdin and removes newline
char *get_input() {
    char buffer[MAX_INPUT];
    fgets(buffer, MAX_INPUT, stdin); // Reads input from user
    buffer[strcspn(buffer, "\n")] = 0; // Removes newline character
    char *input = malloc(strlen(buffer) +1); // Allocates memory
    strcpy(input, buffer); // Copy input into newly allocated memory
    return input; // Returns pointer to input string
}

// Adds a new patient to the array
void addPatient(patient_t *patients, int *num_patients) {
    patient_t *p = &patients[*num_patients]; // Points to next slot

    // Collect patient detials from user input
    printf("Enter name");
    p->name = get_input();

    printf("Enter address: ");
    p->address = get_input();

    printf("Enter phone: ");
    p->phone = get_input();

    printf("Enter email: ");
    p->email = get_input();

    printf("Enter blood type: ");
    p->blood_type = get_input();

    printf("Enter date of birth: ");
    p->date_of_birth = get_input();

    printf("Enter gender: ");
    p->gender = get_input();

    printf("Enter weight: ");
    scanf("%f", &p->weight); // Reads float value for weight
    getchar(); // Consume newline left by scanf

    // Skipping allergies & any other medications patient currently taking for now
    p->allergies = NULL;
    p->medications = NULL;

    (*num_patients)++; // Increment patient count
}

// Adds a new doctor to the array
void addDoctor(doctor_t *doctors, int *num_doctors) {
    doctor_t *d = &doctors[*num_doctors]; // Point to next slot

    // Collect doctor details from user input
    printf("Enter name: ");
    d->name = get_input();

    printf("Enter address: ");
    d->address = get_input();

    printf("Enter phone: ");
    d->phone = get_input();

    printf("Enter email: ");
    d->email = get_input();

    printf("Enter password: ");
    d->password = get_input();

    printf("Enter specialization: ");
    d->specialization = get_input();

    (*num_doctors)++; // Increment doctor count
}

// Frees dynamically allocated memory for a patient
void delete_patient(patient_t *patient) {
    free(patient->name);
    free(patient->address);
    free(patient->phone);
    free(patient->email);
    free(patient->password);
    free(patient->blood_type);
    free(patient->date_of_birth);
    free(patient->gender);
    // Skipping allergies and medications memory freeing
}

// Frees dynamically allocated memory for a doctor
void delete_doctor(doctor_t *doctor) {
    free(doctor->name);
    free(doctor->address);
    free(doctor->phone);
    free(doctor->email);
    free(doctor->password);
    free(doctor->specialization);
}

// Displays all patients in the array
void display_patients(patient_t *patients[], int num_patients) {
    for (int i = 0; i < num_patients; i++) {
        patient_t *p = patients[i]; // Access patient pointer

        printf("Patient %d:\n", i + 1);
        printf("  Name: %s\n", p->name);
        printf("  Address: %s\n", p->address);
        printf("  Phone: %s\n", p->phone);
        printf("  Email: %s\n", p->email);
        printf("  Blood Type: %s\n", p->blood_type);
        printf("  DOB: %s\n", p->date_of_birth);
        printf("  Gender: %s\n", p->gender);
        printf("  Weight: %.2f\n\n", p->weight);
    }
}

// Displays all doctors in the array
void display_doctors(doctor_t *doctors[], int num_doctors) {
    for (int i = 0; i < num_doctors; i++) {
        doctor_t *d = doctors[i]; // Access doctor pointer

        printf("Doctor %d:\n", i + 1);
        printf("  Name: %s\n", d->name);
        printf("  Address: %s\n", d->address);
        printf("  Phone: %s\n", d->phone);
        printf("  Email: %s\n", d->email);
        printf("  Specialization: %s\n\n", d->specialization);
    }
}

// Saves all patient and doctor data to a file
void save_data(patient_t *patients[], int num_patients,
               doctor_t *doctors[], int num_doctors) {
    FILE *f = fopen("data.txt", "w"); // Open file for writing
    if (!f) return; // If file couldn't open, return

    // Write number of patients
    fprintf(f, "%d\n", num_patients);

    // Write each patient's data
    for (int i = 0; i < num_patients; i++) {
        patient_t *p = patients[i];
        fprintf(f, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%f\n",
            p->name, p->address, p->phone, p->email, p->password,
            p->blood_type, p->date_of_birth, p->gender, p->weight);
    }

    // Write number of doctors
    fprintf(f, "%d\n", num_doctors);

    // Write each doctor's data
    for (int i = 0; i < num_doctors; i++) {
        doctor_t *d = doctors[i];
        fprintf(f, "%s\n%s\n%s\n%s\n%s\n%s\n",
            d->name, d->address, d->phone, d->email,
            d->password, d->specialization);
    }

    fclose(f); // Close the file after writing
}

// Loads patient and doctor data from a file
void load_data(patient_t *patients[], int num_patients,
               doctor_t *doctors[], int num_doctors) {
    FILE *f = fopen("data.txt", "r"); // Open file for reading
    if (!f) return; // If file couldn't open, return

    fscanf(f, "%d\n", &num_patients); // Read number of patients
    for (int i = 0; i < num_patients; i++) {
        patient_t *p = malloc(sizeof(patient_t)); // Allocate memory
        char buffer[MAX_INPUT];

        // Read each line and remove newline
        fgets(buffer, MAX_INPUT, f); p->name = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); p->address = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); p->phone = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); p->email = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); p->password = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); p->blood_type = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); p->date_of_birth = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); p->gender = strdup(strtok(buffer, "\n"));
        fscanf(f, "%f\n", &p->weight); // Read weight as float

        p->allergies = NULL; // Skipping for now
        p->medications = NULL; // Skipping for now

        patients[i] = p; // Assign to array
    }

    fscanf(f, "%d\n", &num_doctors); // Read number of doctors
    for (int i = 0; i < num_doctors; i++) {
        doctor_t *d = malloc(sizeof(doctor_t)); // Allocate memory
        char buffer[MAX_INPUT];

        fgets(buffer, MAX_INPUT, f); d->name = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); d->address = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); d->phone = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); d->email = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); d->password = strdup(strtok(buffer, "\n"));
        fgets(buffer, MAX_INPUT, f); d->specialization =
            strdup(strtok(buffer, "\n"));

        doctors[i] = d; // Assign to array
    }

    fclose(f); // Close the file
}

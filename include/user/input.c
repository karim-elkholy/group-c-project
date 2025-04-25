#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

char *strdup_c90(const char *s) {
    char *copy = (char *)malloc(strlen(s) + 1);
    if (copy != NULL) {
        strcpy(copy, s);
    }
    return copy;
}

patient_t *ask_patient_details() {
    patient_t *p = (patient_t *)malloc(sizeof(patient_t));
    char buffer[256];

    printf("Enter name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    p->name = strdup_c90(buffer);

    printf("Enter address: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    p->address = strdup_c90(buffer);

    printf("Enter phone number: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_phone_number(buffer)) {
        printf("Invalid. Re-enter phone number: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    p->phone = strdup_c90(buffer);

    printf("Enter email: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_email(buffer)) {
        printf("Invalid. Re-enter email: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    p->email = strdup_c90(buffer);

    printf("Enter password: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    p->password = strdup_c90(buffer);

    printf("Enter blood type (A, B, AB, O): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_blood_type(buffer)) {
        printf("Invalid. Re-enter blood type: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    p->blood_type = strdup_c90(buffer);

    printf("Enter date of birth (DD/MM/YYYY): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    p->date_of_birth = strdup_c90(buffer);

    printf("Enter gender (Male, Female, Other): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_gender(buffer)) {
        printf("Invalid. Re-enter gender: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    p->gender = strdup_c90(buffer);

    printf("Enter weight (kg): ");
    fgets(buffer, sizeof(buffer), stdin);
    p->weight = (float)atof(buffer);

    p->allergies = NULL;
    p->medications = NULL;

    return p;
}

doctor_t *ask_doctor_details() {
    doctor_t *d = (doctor_t *)malloc(sizeof(doctor_t));
    char buffer[256];

    printf("Enter name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    d->name = strdup_c90(buffer);

    printf("Enter address: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    d->address = strdup_c90(buffer);

    printf("Enter phone number: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_phone_number(buffer)) {
        printf("Invalid. Re-enter phone number: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    d->phone = strdup_c90(buffer);

    printf("Enter email: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    while (!is_valid_email(buffer)) {
        printf("Invalid. Re-enter email: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    d->email = strdup_c90(buffer);

    printf("Enter password: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    d->password = strdup_c90(buffer);

    printf("Enter specialization: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    d->specialization = strdup_c90(buffer);

    return d;
}

int is_valid_blood_type(char *b) {
    return strcmp(b, "A") == 0 || strcmp(b, "B") == 0 ||
           strcmp(b, "AB") == 0 || strcmp(b, "O") == 0;
}

int is_valid_gender(char *g) {
    return strcmp(g, "Male") == 0 || strcmp(g, "Female") == 0 ||
           strcmp(g, "Other") == 0;
}

int is_valid_email(char *e) {
    return strchr(e, '@') != NULL && strchr(e, '.') != NULL;
}

int is_valid_phone_number(char *p) {
    int i;
    for (i = 0; p[i] != '\0'; i++) {
        if (p[i] < '0' || p[i] > '9') return 0;
    }
    return i >= 8;
}

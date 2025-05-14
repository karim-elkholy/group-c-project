#include <stdio.h>

#include "test_shared.h"

void test_patient_signup() {
    printf("Test patient signup\n");
}

void test_patient_signup_silent() {
    printf("Test patient signup silent\n");
}

int main() {

    /* Initialize the dummy hospital */
    hospital_record_t *records = init_dummy_hospital(); 

    /* Get the first patient */
    patient_details_t *patient = records->patients;

    /* Print the patient details */
    printf("Patient details:\n");
    printf("Username: %s\n", patient->username);
    printf("Name: %s\n", patient->name);
    printf("Email: %s\n", patient->email);
    printf("Phone: %s\n", patient->phone);


    return 0;
}

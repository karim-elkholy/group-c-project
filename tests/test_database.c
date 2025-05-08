
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

#include "application/database.h"
#include "application/users/doctor.h"
#include "utils/hash.h"
#include "utils/scanner.h"

void test_run_method( const char *test_name, void (*test_method)()) {
    printf("Running test: %s\n", test_name);
    test_method();
    printf("Test passed [+]\n");
}

/*******************************************************************************
 * Populates the given database with test data.
 * 
 * inputs:
 * - records - The database to populate
 * outputs:
 * - None
 ******************************************************************************/
void test_seed_data(hospital_record_t *records) {

    /* Doctor Homer Simpson */
    doctor_details_t *doctor = (doctor_details_t *)calloc(
        1, sizeof(doctor_details_t)
    );
    strcpy(doctor->username, "1");
    strcpy(doctor->name, "Homer Simpson");
    strcpy(doctor->email, "homer.simpson@example.com");
    strcpy(doctor->phone, "1234567890");
    doctor->password = hash_string("1");
    strcpy(doctor->specialization, "Cardiology");
    strcpy(doctor->license_number, "1234567890");
    doctor_signup_silent(records, doctor);

    /* Patient Bart Simpson */
    patient_details_t *patient = (patient_details_t *)calloc(
        1, sizeof(patient_details_t)
    );
    strcpy(patient->username, "2");
    strcpy(patient->name, "Bart Simpson");
    strcpy(patient->email, "bart.simpson@example.com");
    strcpy(patient->phone, "1234567890");
    patient->password = hash_string("2");
    strcpy(patient->blood_type, "A+");
    strcpy(patient->medical_history, "None");
    strcpy(patient->allergies, "None");
    strcpy(patient->medications, "None");
    patient->weight = 100;
    patient->height = 180;
    strcpy(patient->notes, "None");
    patient_signup_silent(records, patient);

    /* Patient Lisa Simpson */
    patient_details_t *patient2 = (patient_details_t *)calloc(
        1, sizeof(patient_details_t)
    );
    strcpy(patient2->username, "3");
    strcpy(patient2->name, "Lisa Simpson");
    strcpy(patient2->email, "lisa.simpson@example.com");
    strcpy(patient2->phone, "1234567890");
    patient2->password = hash_string("3");
    strcpy(patient2->blood_type, "B-");
    strcpy(patient2->medical_history, "None");
    strcpy(patient2->allergies, "None");
    strcpy(patient2->medications, "None");
    patient2->weight = 100;
    patient2->height = 180;
    strcpy(patient2->notes, "None");
    patient_signup_silent(records, patient2);
}

/*******************************************************************************
 * Tests the load & save database functions.
 * 
 * inputs:
 * - None
 * outputs:
 * - None
 ******************************************************************************/
void test_load_save_database() {

    /* Hospital name */
    const char *hospital_name = "Simpson Hospital";

    /* Load the database */
    hospital_record_t *records = load_database(hospital_name);

    /* Seed the database */
    test_seed_data(records);

    /* Save the database */
    save_database(records);

    /* Load the database again */
    hospital_record_t *records_loaded = load_database(hospital_name);

    /* Check if Homer is found */
    doctor_details_t *doctor = find_doctor(records_loaded, "1");
    if (strcmp(doctor->name, "Homer Simpson") != 0) {
        printf("Test failed\n");
        exit(1);
    }

    /* Check if Bart is found */
    patient_details_t *patient = find_patient(records_loaded, "2");
    if (strcmp(patient->name, "Bart Simpson") != 0) {
        printf("Test failed\n");
        exit(1);
    }

    /* Check if Lisa is found */
    patient_details_t *patient2 = find_patient(records_loaded, "3");
    if (strcmp(patient2->name, "Lisa Simpson") != 0) {
        printf("Test failed\n");
        exit(1);
    }

    /* Close the database */
    close_database(records);
    close_database(records_loaded);
        
        
}

int main() {

    test_run_method("load & save database", test_load_save_database);
    return 0;
}
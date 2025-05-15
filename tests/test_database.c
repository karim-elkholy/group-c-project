
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

#include "application/database.h"
#include "application/users/doctor.h"
#include "application/users/patient.h"
#include "utils/hash.h"
#include "utils/scanner.h"
#include "test_shared.h"

/*******************************************************************************
 * Populates the given database with test data.
 * Alternate seed data.
 * 
 * inputs:
 * - records - The database to populate
 * outputs:
 * - None
 ******************************************************************************/
void test_seed_data_alternate(hospital_record_t *records) {

    /* Doctor Walter White */
    doctor_details_t *doctor = (doctor_details_t *)calloc(
        1, sizeof(doctor_details_t)
    );
    strcpy(doctor->username, "1");
    strcpy(doctor->name, "Walter White");
    strcpy(doctor->email, "walter.white@example.com");
    strcpy(doctor->phone, "1234567890");
    doctor->password = hash_string("1");
    strcpy(doctor->specialization, "Cardiology");
    strcpy(doctor->license_number, "1234567890");
    doctor_signup_silent(records, doctor);

    /* Patient Gus Fring */
    patient_details_t *patient = (patient_details_t *)calloc(
        1, sizeof(patient_details_t)
    );
    strcpy(patient->username, "2");
    strcpy(patient->name, "Gus Fring");
    strcpy(patient->email, "gus.fring@example.com");
    strcpy(patient->phone, "1234567890");
    patient->password = hash_string("2");
    strcpy(patient->blood_type, "A+");
    strcpy(patient->medical_history, "None");
    patient->weight = 100;
    patient->height = 180;
    patient_signup_silent(records, patient);

    /* Patient Hector Salamanca */
    patient_details_t *patient2 = (patient_details_t *)calloc(
        1, sizeof(patient_details_t)
    );
    strcpy(patient2->username, "3");
    strcpy(patient2->name, "Hector Salamanca");
    strcpy(patient2->email, "hector.salamanca@example.com");
    strcpy(patient2->phone, "1234567890");
    patient2->password = hash_string("3");
    strcpy(patient2->blood_type, "B-");
    strcpy(patient2->medical_history, "None");
    patient2->weight = 100;
    patient2->height = 180;
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
    const char *hospital_name = "Breaking Bad Hospital";

    /* Load the database */
    hospital_record_t *records = load_database(hospital_name);
    printf("Loaded database\n");

    /* Seed the database */
    test_seed_data_alternate(records);

    /* Save the database */
    printf("Saving database\n");
    save_database(records);
    printf("Saved database\n");

    /* Load the database again */
    printf("Loading database\n");
    hospital_record_t *records_loaded = load_database(hospital_name);
    printf("Loaded database\n");

    /* Check if Walter is found */
    doctor_details_t *doctor = find_doctor(records_loaded, "1");
    printf("Doctor found: %s\n", doctor->name);
    if (strcmp(doctor->name, "Walter White") != 0) {
        printf("Test failed\n");
        exit(1);
    }

    /* Check if Gus is found */
    patient_details_t *patient = find_patient(records_loaded, "2");
    if (strcmp(patient->name, "Gus Fring") != 0) {
        printf("Test failed\n");
        exit(1);
    }

    /* Check if Hector is found */
    patient_details_t *patient2 = find_patient(records_loaded, "3");
    if (strcmp(patient2->name, "Hector Salamanca") != 0) {
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
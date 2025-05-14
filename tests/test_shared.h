#ifndef TEST_SHARED_H
#define TEST_SHARED_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

#include "application/database.h"
#include "application/users/doctor.h"
#include "application/users/patient.h"
#include "utils/hash.h"
#include "utils/scanner.h"


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
    patient->weight = 100;
    patient->height = 180;
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
    patient2->weight = 100;
    patient2->height = 180;
    patient_signup_silent(records, patient2);
}

/*******************************************************************************
 * Initializes a dummy hospital for testing.
 * 
 * inputs:
 * - None
 * outputs:
 * - records - The dummy hospital
 ******************************************************************************/
hospital_record_t *init_dummy_hospital() {

    /* Hospital name */
    const char *hospital_name = "Simpson Hospital";

    /* Load the database */
    hospital_record_t *records = load_database(hospital_name);

    /* Seed the database */
    test_seed_data(records);

    /* Return the records */
    return records;
}

/*******************************************************************************
 * Closes & deletes the dummy hospital.
 * This includes its database.
 * 
 * inputs:
 * - records - The dummy hospital
 * outputs:
 * - None
 ******************************************************************************/
void close_dummy_hospital(hospital_record_t *records) {
    
    /* Store the name of the database */
    char database_name[256];
    strcpy(database_name, records->encrypted_database_name);

    /* Close the database */
    close_database(records);

    /* Delete the database */
    remove(database_name);
}


/*******************************************************************************
 * Runs a test method.
 * 
 * inputs:
 * - test_name - The name of the test
 * - test_method - The method to run
 * outputs:
 * - None
 ******************************************************************************/
void test_run_method( const char *test_name, void (*test_method)()) {
    printf("Running test: %s\n", test_name);
    test_method();
    printf("Test passed [+]\n");
}

#endif
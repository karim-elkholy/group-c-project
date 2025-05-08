#include <stdio.h>

#include "application/database.h"
#include <stdlib.h>

/*******************************************************************************
 * Initialize the database.
 * 
 * inputs:
 * - hospital_name - The name of the hospital.
 * outputs:
 * - The database.
 ******************************************************************************/
hospital_record_t *database_init(const char *hospital_name) {

    /** Create the database */
    hospital_record_t *records = (hospital_record_t *)malloc(sizeof(hospital_record_t));
    records->num_patients = 0;
    records->num_doctors = 0;

    // Starting number of available slots for patients & doctors
    records->num_patients_available = 100;
    records->num_doctors_available = 100;

    /* Allocate memory to hold 100 patients & 100 doctors*/
    records->patients = (patient_details_t *)malloc(
        records->num_patients_available * sizeof(patient_details_t));
    records->doctors = (doctor_details_t *)malloc(
        records->num_doctors_available * sizeof(doctor_details_t));

    // Return the database
    return records;
}


/*******************************************************************************
 * Load the database.
 * 
 * inputs:
 * - hospital_name - The name of the hospital.
 * outputs:
 * - The database.
 ******************************************************************************/
hospital_record_t *load_database(const char *hospital_name, int *num_users) {

    // Initialize the database
    hospital_record_t *records = database_init(hospital_name);
    
    /* Name of database file */
    char db_name[256];
    sprintf(db_name, "%s.db", hospital_name);

    /* Open the database file */
    FILE *db = fopen(db_name, "r");

    /* If the database file was not opened successfully */
    if (db == NULL) {
        /* Assume the database is empty */
        return records;
    }

    /* TODO - Later decrypt the database */

    /* Set the number of patients in the database */
    fscanf(db, "%d", &records->num_patients);

    /* If there is not enough memory for the patients */
    if (records->num_patients > records->num_patients_available) {
        /* Allocate more memory to hold more patients */
        free(records->patients);
        records->patients = (patient_details_t *)realloc(
            records->patients, records->num_patients * sizeof(patient_details_t));
        records->num_patients_available = records->num_patients;
    }

    /* Load the patients from the database */
    int i;
    for (i = 0; i < records->num_patients; i++) {
        fscanf(db, "%s", records->patients[i].id);
        fscanf(db, "%s", records->patients[i].name);
        fscanf(db, "%s", records->patients[i].email);
        fscanf(db, "%s", records->patients[i].phone);
        fscanf(db, "%s", records->patients[i].password);

        fscanf(db, "%s", records->patients[i].blood_type);
        fscanf(db, "%s", records->patients[i].medical_history);
        fscanf(db, "%s", records->patients[i].allergies);
        fscanf(db, "%s", records->patients[i].medications);
        fscanf(db, "%f", &records->patients[i].weight);
        fscanf(db, "%f", &records->patients[i].height);
        fscanf(db, "%s", records->patients[i].notes);
    }

    /* Read all the doctors from the database */
    fscanf(db, "%d", &records->num_doctors);

    /* If there is not enough memory for the doctors */
    if (records->num_doctors > records->num_doctors_available) {
        /* Allocate more memory to hold more doctors */
        free(records->doctors);
        records->doctors = (doctor_details_t *)realloc(
            records->doctors, records->num_doctors * sizeof(doctor_details_t));
        records->num_doctors_available = records->num_doctors;
    }

    /* Load the doctors from the database */
    for (i = 0; i < records->num_doctors; i++) {
        fscanf(db, "%s", records->doctors[i].id);
        fscanf(db, "%s", records->doctors[i].name);
        fscanf(db, "%s", records->doctors[i].email);
        fscanf(db, "%s", records->doctors[i].phone);
        fscanf(db, "%u", &records->doctors[i].password);
        fscanf(db, "%s", records->doctors[i].specialization);
        fscanf(db, "%s", records->doctors[i].license_number);
    }

    /* Close the database file */
    fclose(db);

    /* Return the list of users */
    return records;
}


/*******************************************************************************
 * Save the database.
 * 
 * inputs:
 * - hospital_name - The name of the hospital.
 * - records - The hospital records.
 * outputs:
 * - None.
 *******************************************************************************/
void save_database(const char *hospital_name, hospital_record_t *records) {

    /* Name of database file */
    char db_name[256];
    sprintf(db_name, "%s.db", hospital_name);

    /* Open the database file */
    FILE *db = fopen(db_name, "w");

    /* Failure to open the database file should cause the program to exit.*/
    if (db == NULL) {
        printf("Error: Failed to open stored database file.\n");
        exit(1);
    }

    /* TODO - Later encrypt the database */

    /* Write the number of patients to the database */
    fprintf(db, "%d\n", records->num_patients);

    /* Write the patients to the database */
    int i;
    for (i = 0; i < records->num_patients; i++) {
        fprintf(db, "%s\n", records->patients[i].id);
        fprintf(db, "%s\n", records->patients[i].name);
        fprintf(db, "%s\n", records->patients[i].email);
        fprintf(db, "%s\n", records->patients[i].phone);
        fprintf(db, "%s\n", records->patients[i].password);
        fprintf(db, "%s\n", records->patients[i].blood_type);
        fprintf(db, "%s\n", records->patients[i].medical_history);
        fprintf(db, "%s\n", records->patients[i].allergies);
        fprintf(db, "%s\n", records->patients[i].medications);
        fprintf(db, "%f\n", records->patients[i].weight);
        fprintf(db, "%f\n", records->patients[i].height);
        fprintf(db, "%s\n", records->patients[i].notes);
    }

    /* Write the number of doctors to the database */
    fprintf(db, "%d\n", records->num_doctors);

    /* Write the doctors to the database */
    for (i = 0; i < records->num_doctors; i++) {
        fprintf(db, "%s\n", records->doctors[i].id);
        fprintf(db, "%s\n", records->doctors[i].name);
        fprintf(db, "%s\n", records->doctors[i].email);
        fprintf(db, "%s\n", records->doctors[i].phone);
        fprintf(db, "%u\n", records->doctors[i].password);
        fprintf(db, "%s\n", records->doctors[i].specialization);
        fprintf(db, "%s\n", records->doctors[i].license_number);
    }
}


/*******************************************************************************
 * Free the database.
 * 
 * inputs:
 * - records - The hospital records.
 * outputs:
 * - None.
 *******************************************************************************/
void close_database(hospital_record_t *records) {
    free(records->patients);
    free(records->doctors);
    free(records);
}

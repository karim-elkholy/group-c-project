#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "application/database.h"
#include "utils/compression.h"

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
    hospital_record_t *records = (hospital_record_t *)malloc(
        sizeof(hospital_record_t));
    records->num_patients = 0;
    records->num_doctors = 0;

    /* Hospital name */
    strcpy(records->hospital_name, hospital_name);

    /* Set the linked lists to NULL since by default no values are present */
    records->doctors = NULL;
    records->patients = NULL;

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
hospital_record_t *load_database(const char *hospital_name) {

    /* Initialize the database */
    hospital_record_t *records = database_init(hospital_name);

    /* Name of database file */
    char db_name[256];
    sprintf(db_name, "%s.db", hospital_name);

    /* Decompress the database(if one already exists) */
    decompress_file("compressed.db", db_name);

    /* Open the database file */
    FILE *db = fopen(db_name, "rb");

    /* If the database file was not opened successfully */
    if (db == NULL) {
        /* Assume the database is empty */
        return records;
    }

    /* TODO - Later decrypt the database */

    /* -----------------------------------------------------------------------*/
    /* Doctors section */
    /* -----------------------------------------------------------------------*/
    /* Read all the doctors from the database */
    fread(&records->num_doctors, sizeof(int), 1, db);

    /* Linked list for doctors */
    /* Keeps track of the last doctor in the linked list */
    doctor_details_t *doctors_tail = NULL;

    /* Load all the doctors from the database */
    int i;
    for (i = 0; i < records->num_doctors; i++) {

        /* Allocate memory to hold the new doctor */
        doctor_details_t *doctor = (doctor_details_t *)calloc(
            1, sizeof(doctor_details_t)
        );
        
        /* Read each field from the database */
        /* Username */
        fread(doctor->username, sizeof(char), 256, db);
        /* Name */
        fread(doctor->name, sizeof(char), 256, db);
        /* Email */
        fread(doctor->email, sizeof(char), 256, db);
        /* Phone */
        fread(doctor->phone, sizeof(char), 256, db);
        /* Password */
        fread(&doctor->password, sizeof(unsigned int), 1, db);
        /* Specialization */
        fread(doctor->specialization, sizeof(char), 256, db);
        /* License number */
        fread(doctor->license_number, sizeof(char), 256, db);

        /* If this is the first entry of the linked list */
        if ( records->doctors == NULL ) {
            records->doctors = doctor;
            doctors_tail = doctor;

        /* For all other entries */
        } else {
            /* Update the tail to the new doctor */
            doctors_tail->next = doctor;

            /* Prepare to store the next doctor */
            doctors_tail = doctors_tail->next;
        }
    }

    /* -----------------------------------------------------------------------*/
    /* Patients section */
    /* -----------------------------------------------------------------------*/

    /* Read all the patients from the database */
    fread(&records->num_patients, sizeof(int), 1, db);

    /* Linked list for patients */
    /* Keeps track of the last patient in the linked list */
    patient_details_t *patients_tail = NULL;

    /* Load all the patients from the database */
    for (i = 0; i < records->num_patients; i++) {

        /* Allocate memory to hold the new patient */
        patient_details_t *patient = (patient_details_t *)calloc(
            1, sizeof(patient_details_t)
        );
        
        /* Read each field from the database */
        /* Username */
        fread(patient->username, sizeof(char), 256, db);
        /* Name */
        fread(patient->name, sizeof(char), 256, db);
        /* Email */
        fread(patient->email, sizeof(char), 256, db);
        /* Phone */
        fread(patient->phone, sizeof(char), 256, db);
        /* Password */
        fread(&patient->password, sizeof(unsigned int), 1, db);
        /* Blood type */
        fread(patient->blood_type, sizeof(char), 3, db);
        /* Medical history */
        fread(patient->medical_history, sizeof(char), 256, db);
        /* Allergies */
        fread(patient->allergies, sizeof(char), 256, db);
        /* Medications */
        fread(patient->medications, sizeof(char), 256, db);
        /* Weight */
        fread(&patient->weight, sizeof(float), 1, db);
        /* Height */
        fread(&patient->height, sizeof(float), 1, db);
        /* Notes */
        fread(patient->notes, sizeof(char), 256, db);


        /* If this is the first entry of the linked list */
        if ( records->patients == NULL ) {
            records->patients = patient;
            patients_tail = patient;

        /* For all other entries */
        } else {
            /* Update the tail to the new patient */
            patients_tail->next = patient;

            /* Prepare to store the next patient */
            patients_tail = patients_tail->next;
        }
    }

    /* Close the database file */
    fclose(db);

    /* Remove the database file */
    /* remove(db_name); */

    /* Return the list of users */
    return records;
}

/*******************************************************************************
 * Save the database.
 * 
 * inputs:
 * - records - The hospital records.
 * outputs:
 * - None.
 ******************************************************************************/
void save_database(hospital_record_t *records) {

    /* Name of database file */
    /* 3 extra characters added to accommodate the .db extension */
    char db_name[259];
    sprintf(db_name, "%s.db", records->hospital_name);

    /* Open the database file */
    FILE *db = fopen(db_name, "wb");

    /* Failure to open the database file should cause the program to exit.*/
    if (db == NULL) {
        printf("Error: Failed to open stored database file.\n");
        exit(1);
    }

    /* TODO - Later encrypt the database */

    /* -----------------------------------------------------------------------*/
    /* Doctors section */
    /* -----------------------------------------------------------------------*/

    /* Write the number of doctors to the database */
    fwrite(&records->num_doctors, sizeof(int), 1, db);

    /* Linked list for doctors */
    doctor_details_t *doctors = records->doctors;

    /* Add each node to the database */
    while (doctors != NULL) {
       /* Write each field to the database */
       /* Username*/
        fwrite(doctors->username, sizeof(char), 256, db);
        /* Name */
        fwrite(doctors->name, sizeof(char), 256, db);
        /* Email */
        fwrite(doctors->email, sizeof(char), 256, db);
        /* Phone */
        fwrite(doctors->phone, sizeof(char), 256, db);
        /* Password */
        fwrite(&doctors->password, sizeof(unsigned int), 1, db);
        /* Specialization */
        fwrite(doctors->specialization, sizeof(char), 256, db);
        /* License number */
        fwrite(doctors->license_number, sizeof(char), 256, db);
        
        /* Move to the next node */
        doctors = doctors->next;
    }

    /* -----------------------------------------------------------------------*/
    /* Patient section */
    /* -----------------------------------------------------------------------*/

    /* Write the number of patients to the database */
    fwrite(&records->num_patients, sizeof(int), 1, db);

    /* Linked list for patients */
    patient_details_t *patients = records->patients;

    /* Add each node to the database */
    while (patients != NULL) {
        /* Write each field to the database */
        /* Username */
        fwrite(patients->username, sizeof(char), 256, db);
        /* Name */
        fwrite(patients->name, sizeof(char), 256, db);
        /* Email */
        fwrite(patients->email, sizeof(char), 256, db);
        /* Phone */
        fwrite(patients->phone, sizeof(char), 256, db);
        /* Password */
        fwrite(&patients->password, sizeof(unsigned int), 1, db);
        /* Blood type */
        fwrite(patients->blood_type, sizeof(char), 3, db);
        /* Medical history */
        fwrite(patients->medical_history, sizeof(char), 256, db);
        /* Allergies */
        fwrite(patients->allergies, sizeof(char), 256, db);
        /* Medications */
        fwrite(patients->medications, sizeof(char), 256, db);
        /* Weight */
        fwrite(&patients->weight, sizeof(float), 1, db);
        /* Height */
        fwrite(&patients->height, sizeof(float), 1, db);
        /* Notes */
        fwrite(patients->notes, sizeof(char), 256, db);

        /* Move to the next patient */
        patients = patients->next;
    }

    /* Close the database file */
    fclose(db);

    /* Compress the database */
    printf("Compressing database\n");
    compress_file(db_name, "compressed.db");
    printf("Compressed database\n");
    
    /* Remove the database file */
    /* remove(db_name); */
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

    /* Free the linked list of patients */
    patient_details_t *patients = records->patients;
    while (patients != NULL) {
        /* Store the next node before freeing the current node */
        patient_details_t *next = patients->next;
        free(patients);

        /* Move to the next node */
        /* Loop will continue until the next node is NULL */
        patients = next;
    }

    /* Free the linked list of doctors */
    doctor_details_t *doctors = records->doctors;
    while (doctors != NULL) {

        /* Store the next node before freeing the current node */
        doctor_details_t *next = doctors->next;
        free(doctors);

        /* Move to the next node */
        /* Loop will continue until the next node is NULL */
        doctors = next;
    }

    free(records);
}

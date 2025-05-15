#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "application/database.h"
#include "utils/compression.h"
#include "utils/hex.h"
#include "encryption/aes/gcm.h"

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

    records->patients = NULL;
    records->num_patients = 0;
    records->doctors = NULL;
    records->num_doctors = 0;

    /* Initialize 10 beds */
    records->beds = (bed_details_t *)malloc(10 * sizeof(bed_details_t));
    records->num_beds = 10;
    records->num_beds_in_use = 0;

    /* Initialize the beds since by default no patients are assigned to them */
    int i;
    for (i = 0; i < records->num_beds; i++) {
        records->beds[i].patient = NULL;
    }

    /* Hospital name */
    strcpy(records->hospital_name, hospital_name);

    /* Set the database name */
    strcpy(records->encrypted_database_name, hospital_name);
    strcat(records->encrypted_database_name, "_encrypted.db");

    /* Set the linked lists to NULL since by default no values are present */
    records->doctors = NULL;
    records->patients = NULL;

    /* Return the database */
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

    /* Create encryption key & nonce */
    unsigned char *key = convert_hex_string_to_bytes(
        "feffe9928665731c6d6a8f9467308308");
    unsigned char *nonce = convert_hex_string_to_bytes(
        "cafebabefacedbaddecaf888");
    int key_size = 16;

    /* Initialize the database */
    hospital_record_t *records = database_init(hospital_name);

    /* Name of database file(s) */
    char db_name[256];
    strcpy(db_name, hospital_name);
    strcat(db_name, "_encrypted.db");
    char db_name_compressed[256];
    strcpy(db_name_compressed, hospital_name);
    strcat(db_name_compressed, "_compressed.db");

    /* If the database does not exist */
    FILE *encrypted_db = fopen(records->encrypted_database_name, "rb");
    if (encrypted_db == NULL) {
     
        /* Assume no database exists yet*/
        return records;
    }

    /* Decrypt the database */
    aes_gcm_decrypt_file(
        records->encrypted_database_name,
        db_name_compressed,
        key, key_size,
        NULL, 0,
        nonce);

    /* Decompress the database */
    /* decompress_file(db_name_compressed, db_name); */
    decompress_huffman(db_name_compressed, db_name); 


    /* Open the database file */
    FILE *db = fopen(db_name, "rb");

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
        /* Weight */
        fread(&patient->weight, sizeof(float), 1, db);
        /* Height */
        fread(&patient->height, sizeof(float), 1, db);
        /* BMI */
        fread(&patient->bmi, sizeof(float), 1, db);

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

    /* Close the files opened */
    fclose(db);
    fclose(encrypted_db);

    /* Remove the compressed database file */
    if (remove(db_name_compressed) != 0) {
        printf("Error: Failed to delete %s\n", db_name_compressed);
    }

    /* Remove the database file */
    if (remove(db_name) != 0) {
        printf("Error: Failed to delete %s\n", db_name);
    }

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

    /* Create encryption key & nonce */
    unsigned char *key = convert_hex_string_to_bytes(
        "feffe9928665731c6d6a8f9467308308");
    unsigned char *nonce = convert_hex_string_to_bytes(
        "cafebabefacedbaddecaf888");
    int key_size = 16;

    /* Name of database file(s) */
    char db_name[256];
    strcpy(db_name, records->hospital_name);
    strcat(db_name, ".db");
    /* Compressed database name */
    char db_name_compressed[256];
    strcpy(db_name_compressed, records->hospital_name);
    strcat(db_name_compressed, "_compressed.db");

    /* Open the database file */
    FILE *db = fopen(db_name, "wb");

    /* Failure to open the database file should cause the program to exit.*/
    if (db == NULL) {
        printf("Error: Failed to open stored database file.\n");
        exit(1);
    }

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
        /* Weight */
        fwrite(&patients->weight, sizeof(float), 1, db);
        /* Height */
        fwrite(&patients->height, sizeof(float), 1, db);
        /* BMI */
        fwrite(&patients->bmi, sizeof(float), 1, db);

        /* Move to the next patient */
        patients = patients->next;
    }

    /* Close the database file */
    fclose(db);

    /* Compress the database */
    /* compress_file(db_name, db_name_compressed); */
    compression_huffman(db_name, db_name_compressed);

    /* Encrypt the database */
    aes_gcm_encrypt_file(
        db_name_compressed,
        records->encrypted_database_name,
        key, key_size,
        NULL, 0,
        nonce);
    
    /* Remove all database file(s) except the encrypted database file */
    if (remove(db_name_compressed) != 0) {
        printf("Error: Failed to delete %s\n", db_name_compressed);
    }

    if (remove(db_name) != 0) {
        printf("Error: Failed to delete %s\n", db_name);
    }

}


/*******************************************************************************
 * Free the database.
 * 
 * inputs:
 * - records - The hospital records.
 * outputs:
 * - None.
 ******************************************************************************/
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

    /* Free the list of beds */
    free(records->beds);

    /* Free the records */
    free(records);
}


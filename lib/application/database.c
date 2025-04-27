
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/scanner.h"
#include "utils/hash.h"
#include "application/users/user.h"
#include "application/users/records.h"
/*******************************************************************************
 * Load the common user details shared by both entities(patients and doctors).
 * Common details include: id, name, email, phone, password hash.
 * 
 * inputs:
 * - db - The database file.
 * - user - The user to load the details into.
 * outputs:
 * - None.
 ******************************************************************************/
void load_common_user_details(FILE *db, user_t *user) {
    fscanf(db, "%s", user->id);
    fscanf(db, "%s", user->name);
    fscanf(db, "%s", user->email);
    fscanf(db, "%s", user->phone);
    fscanf(db, "%u", &user->password);
}

/*******************************************************************************
 * Load the database.
 * 
 * inputs:
 * - hospital_name - The name of the hospital.
 * outputs:
 * - The database.
 ******************************************************************************/
record_t *load_database(const char *hospital_name, int *num_users) {

    /* Allocate list of users */
    record_t *records = (record_t *)malloc(sizeof(record_t));
    
    /* Name of database file */
    char db_name[256];
    sprintf(db_name, "%s.db", hospital_name);

    /* Open the database file */
    FILE *db = fopen(db_name, "r");

    /* If the database file was not opened successfully */
    if (db == NULL) {
        /* Assume the database is empty */
        *num_users = 0;
        return records;
    }

    /* TODO - Later decrypt the database */

    /* Set the number of users in the database */
    fscanf(db, "%d", &records->num_patients);

    /* Read all the patients from the database */
    int i;
    for (i = 0; i < records->num_patients; i++) {
        /* Common user details */
        load_common_user_details(db, &records->patients[i].user);

        /* Patient details */
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
    for (i = 0; i < records->num_doctors; i++) {
        load_common_user_details(db, &records->doctors[i].user);
        fscanf(db, "%s", records->doctors[i].specialization);
        fscanf(db, "%s", records->doctors[i].license_number);
    }

    /* Return the list of users */
    return records;
}

/*******************************************************************************
 * Save the common user details shared by both entities(patients and doctors).
 * Common details include: id, name, email, phone, password hash.
 * 
 * inputs:
 * - db - The database file.
 * - user - The user to save the details of.
 * outputs:
 * - None.
 ******************************************************************************/
void save_common_user_details(FILE *db, user_t *user) {
    fprintf(db, "%s\n", user->id);
    fprintf(db, "%s\n", user->name);
    fprintf(db, "%s\n", user->email);
    fprintf(db, "%s\n", user->phone);
    fprintf(db, "%u\n", user->password);
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
void save_database(const char *hospital_name, record_t *records) {

    /* Name of database file */
    char db_name[256];
    sprintf(db_name, "%s.db", hospital_name);

    /* Open the database file */
    FILE *db = fopen(db_name, "w");

    /* Failure to open the database file should cause the program to exit.*/
    if (db == NULL) {
        printf("Error: Failed to open database file.\n");
        exit(1);
    }

    /* TODO - Later encrypt the database */

    /* Write the number of patients to the database */
    fprintf(db, "%d\n", records->num_patients);

    /* Write the patients to the database */
    int i;
    for (i = 0; i < records->num_patients; i++) {
        /* Common user details */
        save_common_user_details(db, &records->patients[i].user);

        /* Patient details */
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
        /* Common user details */
        save_common_user_details(db, &records->doctors[i].user);

        /* Doctor details */
        fprintf(db, "%s\n", records->doctors[i].specialization);
        fprintf(db, "%s\n", records->doctors[i].license_number);
    }
}




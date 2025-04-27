#ifndef APPLICATION_DATABASE_H
#define APPLICATION_DATABASE_H

#include "application/users/records.h"


/*******************************************************************************
 * Load the database.
 * 
 * inputs:
 * - hospital_name - The name of the hospital
 * - num_users - The number of users in the database
 * outputs:
 * - The database
 ******************************************************************************/
record_t *load_database(const char *hospital_name, int *num_users);


/*******************************************************************************
 * Save the database.
 * 
 * inputs:
 * - hospital_name - The name of the hospital
 * - records - The database
 * outputs:
 * - none
 ******************************************************************************/
void save_database(const char *hospital_name, record_t *records);

#endif


/*******************************************************************************
 * File: input.h
 * Description: Function declarations for user input and validation routines.
 * Author: Keiron Lee
 ******************************************************************************/

#ifndef USER_INPUT_H
#define USER_INPUT_H

#include "core.h"

/*******************************************************************************
 * Input Collection Functions
 * Author: Keiron Lee
 ******************************************************************************/

patient_t *ask_patient_details();
doctor_t *ask_doctor_details();

/*******************************************************************************
 * Input Validation Functions
 * Author: Keiron Lee
 ******************************************************************************/

int is_valid_blood_type(char *blood_type);
int is_valid_gender(char *gender);
int is_valid_email(char *email);
int is_valid_phone_number(char *phone_number);

/*******************************************************************************
 * User Interaction Entry Point
 * Author: Keiron Lee
 ******************************************************************************/

void use();

#endif

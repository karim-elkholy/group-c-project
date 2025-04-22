#ifndef USER_INPUT_H
#define USER_INPUT_H


#include "core.h"

patient_t *ask_patient_details();

doctor_t *ask_doctor_details();

int is_valid_blood_type(char *blood_type);

int is_valid_gender(char *gender);

int is_valid_email(char *email);

int is_valid_phone_number(char *phone_number);

/* Entry point for the program. Will prompt choices from here */
void use();

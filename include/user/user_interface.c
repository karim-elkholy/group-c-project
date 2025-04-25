/*******************************************************************************
 * File: user_interface.c
 * Description: User interface for the hospital management CLI app.
 * Author: Keiron Lee
 ******************************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include "input.h"
 #include "core.h"
 
 #define MAX_PATIENTS 100
 #define MAX_DOCTORS 100
 
 static patient_t *patients[MAX_PATIENTS];
 static doctor_t *doctors[MAX_DOCTORS];
 static int num_patients = 0;
 static int num_doctors = 0;
 
 /*******************************************************************************
  * Function: patient_menu
  * ----------------------
  * Presents options for managing patients: add, display, or return.
  * Author: Keiron Lee
  ******************************************************************************/
 void patient_menu() {
     int choice;
     do {
         printf("\nPatient Menu:\n");
         printf("1. Add Patient\n");
         printf("2. Display Patients\n");
         printf("3. Return to Main Menu\n");
         printf("Enter choice: ");
         scanf("%d", &choice);
         getchar();
 
         if (choice == 1 && num_patients < MAX_PATIENTS) {
             patients[num_patients] = ask_patient_details();
             num_patients++;
         } else if (choice == 2) {
             display_patients(patients, num_patients);
         }
     } while (choice != 3);
 }
 
 /*******************************************************************************
  * Function: doctor_menu
  * ---------------------
  * Presents options for managing doctors: add, display, or return.
  * Author: Keiron Lee
  ******************************************************************************/
 void doctor_menu() {
     int choice;
     do {
         printf("\nDoctor Menu:\n");
         printf("1. Add Doctor\n");
         printf("2. Display Doctors\n");
         printf("3. Return to Main Menu\n");
         printf("Enter choice: ");
         scanf("%d", &choice);
         getchar();
 
         if (choice == 1 && num_doctors < MAX_DOCTORS) {
             doctors[num_doctors] = ask_doctor_details();
             num_doctors++;
         } else if (choice == 2) {
             display_doctors(doctors, num_doctors);
         }
     } while (choice != 3);
 }
 
 /*******************************************************************************
  * Function: use
  * -------------
  * Entry point for the app. Presents main menu and links to other sections.
  * Author: Keiron Lee
  ******************************************************************************/
 void use() {
     int choice;
     do {
         printf("\n--- Hospital App Main Menu ---\n");
         printf("1. Patient Menu\n");
         printf("2. Doctor Menu\n");
         printf("3. Save Data\n");
         printf("4. Load Data\n");
         printf("5. Exit\n");
         printf("Enter choice: ");
         scanf("%d", &choice);
         getchar();
 
         if (choice == 1) {
             patient_menu();
         } else if (choice == 2) {
             doctor_menu();
         } else if (choice == 3) {
             save_data(patients, num_patients, doctors, num_doctors);
         } else if (choice == 4) {
             load_data(patients, MAX_PATIENTS, doctors, MAX_DOCTORS);
         }
     } while (choice != 5);
 }
 

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 6) {
        char *mode = argv[1];
        char *doctor_username = argv[2];
        char *doctor_password = argv[3];
        char *p_flag = argv[4];
        char *patient_username = argv[5];

        if (strcmp(p_flag, "-p") != 0) {
            printf("Usage:\n");
            printf("  program_name.exe -V <doctor_user> <doctor_pass> -p <patient_user>\n");
            printf("  program_name.exe -D <doctor_user> <doctor_pass> -p <patient_user>\n");
        }

        // command 1 - View patient
        if (strcmp(mode, "-V") == 0) {             
            printf("%s %s %s\n", doctor_username, doctor_password, patient_username);
        }  else if (strcmp(mode, "-D") == 0 ) { // command 2 - Delete patient
            printf("%s %s %s\n", doctor_username, doctor_password, patient_username);
        } else {
            printf("Invalid arguments. Use -V/-D and -p correctly.\n");
        }scanf  
        
    } else {
        printf("Usage:\n");
        printf("  program_name.exe -V <doctor_user> <doctor_pass> -p <patient_user>\n");
        printf("  program_name.exe -D <doctor_user> <doctor_pass> -p <patient_user>\n");
    }

    return 0;
}


#ifndef APPLICATION_USERS_USER_H
#define APPLICATION_USERS_USER_H

/* User details
* Holds common details shared by multiple entities(patients and doctors) 
*/
typedef struct user {
    /* ID */
    char id[256];
    
    /* Name */
    char name[256];
    
    /* Email */
    char email[256];

    /* Phone */
    char phone[256];

    /* Hashed password */
    unsigned int password;
} user_t;

#endif


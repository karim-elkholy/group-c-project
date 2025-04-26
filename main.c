#include "include/aes/core/aes.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include "include/user/user_interface.h"

int main()
{
    /* Name of the hospital */
    const char *hospital_name = "C Hospital";

    /* Use the hospital */
    use(hospital_name);

    return 0;
}
